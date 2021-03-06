/*
  -------------------------------------------------------------------
  
  Copyright (C) 2013, Edwin van Leeuwen
  
  This file is part of chainmcmc.
  
 	chainmcmc is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 3 of the License, or
  (at your option) any later version.
  
  chainmcmc is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with chainmcmc. If not, see <http://www.gnu.org/licenses/>.

  -------------------------------------------------------------------
*/

#ifndef CHAIN_HH
#define CHAIN_HH

#include<vector>

#include <cppa/cppa.hpp>
#include <random>
#include "src/chainmcmc/trace.hh"
#include "src/chainmcmc/logger.hh"
#include "src/chainmcmc/prior.hh"
#include "src/chainmcmc/temperature.hh"

namespace chainmcmc {
using namespace cppa;


/**
 * Parallel Metropolis Coupled Markov Chain Monte Carlo for Bayesian Phylogenetic Inference, 2004, Altekar et all.*/

/**
 * \brief Chain that runs mcmc process
 *
 * Chain is an actor and listens too certain commands (run no_generations
 * set temp, get temp)
 *
 * loglikelihood function, which takes parameters and data
 * parameters
 * priors, should always first do prior before calling loglikelihood, to see if it is usefull at all
 * temperature
 * data
 */

typedef std::function<double( const std::vector<parameter_t> )> likelihood_t;

namespace step {

	double mh_log_weight( const likelihood_t &ll,
			const std::vector<parameter_t> &pars,
			const joint_prior_t &joint_prior, const double &temperature );

	double mh_log_weight( const double &log_likelihood,
			const std::vector<parameter_t> &pars,
			const joint_prior_t &joint_prior, 
			const double &temperature );

	bool accept( std::mt19937 &eng, 
			const double &old_lweight,
			const double &new_lweight,
			const double &temperature = 1 );

	bool accept( std::mt19937 &eng, 
			const likelihood_t &ll, const double &old_ll,
			const std::vector<parameter_t> &old_pars,
			const std::vector<parameter_t> &new_pars,
			const joint_prior_t &joint_prior, double temperature = 1 );	

	bool accept( std::mt19937 &eng, const likelihood_t &ll, 
			const std::vector<parameter_t> &old_pars,
			const std::vector<parameter_t> &new_pars,
			const joint_prior_t &joint_prior, double temperature = 1 );

	parameter_t rparameter( std::mt19937 &eng, const parameter_t &par, 
			const double &sd );

	/**
	 * \brief Adapt the step size
	 *
	 * \param minimum_tries Only start adjusting after this number of tries
	 * \param min_level Adjust step size if acceptance rate below this level
	 * \param max_level Adjust step size if acceptance rate above this level
	 * \param interval Only adjust every so often. This basically slows down adapting and hopefully makes it less likely to over compensate
	 */
	double adapt_step_size( const double current_step_size,
			const size_t no_tries, const size_t no_accepts,
			const size_t minimum_tries, const double min_level, const double max_level,
			const size_t interval = 1 );

	class ParameterState {
		public:
			size_t no_accepts = 0;
			size_t no_tries = 0;
			double sd = 0.001;
	};

	ParameterState adapt_parameter_sd( ParameterState && ps );

	/**
	 * \brief Stores the state of our mcmc chain
	 */
	class State {
		public:
			size_t generation = 0;
			double loglikelihood = NAN;
			std::vector<parameter_t> parameters;
			size_t current_parameter = 0; //! The parameter index we need to change
			std::vector<ParameterState> pss;
	};

	State step( std::mt19937 &eng, State && state, const likelihood_t &ll, 
			const joint_prior_t &joint_prior, bool adapting = true, 
			double temperature = 1 );
};

class Chain : public event_based_actor {
	public:
		Chain() {};
		/**
		 * \brief Start new chain
		 *
		 * \param loglikelihood function that calculates loglikelihood given a vector of parameters.
		 */
		Chain(  std::mt19937 &engine, 
		const likelihood_t &loglikelihood, 
		const std::vector<parameter_t> &parameters,
		const std::vector<prior_t> &priors,
		const double & temperature = 1 );

		Chain(  std::mt19937 &engine, 
		const likelihood_t &loglikelihood, 
		const std::vector<parameter_t> &parameters,
		const joint_prior_t &joint_prior,
		const double & temperature = 1 );

		void init();

	protected:
		std::mt19937 rnd_engine;
		step::State state;
		double temperature = 1;
		const likelihood_t loglikelihood;
		joint_prior_t joint_prior;

		bool log_on = false;
		bool adapting = true;

		actor_ptr logger = spawn<Logger>( std::cout );

		void step();
};

template<class T>
std::vector<T> fisherYatesKSubsets( std::vector<T> &v, 
		size_t k, std::mt19937 &rnd_engine ) { 
	size_t n = v.size()-1;
	for (size_t i=0; i<k; ++i) {
		std::uniform_int_distribution<size_t> distribution( 0, n );
		size_t j = distribution(rnd_engine);
		std::swap( v[j], v[n] );
		--n;
	}
	std::vector<T> ans(v.end()-k, v.end());
	return ans; 
};


/**
 * \brief Chain Controller to implement power posteriors
 *
 * This method provides both hot cold chain mixing, for searching through parameter space and allows us to calculate the posterior probability distribution of the model, useful for model comparison.
 *
 * Following:
		Friel, N., and A. N. Pettitt. 2008. “Marginal Likelihood Estimation via Power Posteriors.” Journal of the Royal Statistical Society: Series B (Statistical Methodology) 70 (3): 589–607. doi:10.1111/j.1467-9868.2007.00650.x.

 * Some implementation notes
 * Keep a population of chains with different ts, also keep a map of each t 
 * with a trace. Ideally the trace should contain samples and their 
 * likelihood value. Actually since we need flat log likelihood and not the hot values, it is not that useful to save the likelihood value. 
 * This map can be used at the end to calculate the posterior probability 
 * following Friel and Pettitt
 */
class FPChainController {
	public:
		std::mt19937 engine;

		FPChainController( const likelihood_t &loglikelihood, 
				const std::vector<std::vector<parameter_t> > &pars_v,
				const joint_prior_t &jp, size_t warm_up, size_t total_steps, 
				std::ostream &out = std::cout );
		
		/**
		 * \brief Run the mcmc and returns a map of temperatures and mean log likelihoods
		 *
		 * This map is used to calculate the marginal log likelihood
		 */
		std::map<double, double> run();

		double integrate( const std::map<double, double>& es );
		std::map<double, std::vector<trace::sample_t> > traces;

		/**
		 * \brief Return the log marginal likelihood of the model
		 *
		 * Alias of integrate
		 *
		 * \param estimated_lls are the estimate log likelihood returned by run()
		 */
		double log_marginal_likelihood( 
				const std::map<double, double>& estimated_lls );

	protected:
		//! Number of populations (see Friel and Pettitt 2008)
		size_t n = 50;
		//! Distribution of populations (see Friel and Pettitt 2008)
		size_t c = 5;

		size_t no_steps_between_swaps = 10; //! Try swap after this many steps

		size_t warm_up = 10000;
		size_t total_steps = 10000;

		std::map<size_t, temperature::ChainState> chains;

		likelihood_t log_likelihood;
		joint_prior_t joint_prior;

		std::ostream &out;

		void setup( const likelihood_t &loglikelihood, 
				const std::vector<std::vector<parameter_t> > &pars_v,
				const joint_prior_t &joint_prior, std::ostream &out );

};

/**
 * \brief Control the hot and cold chains
 *
 * This is a hot-cold chain implementation of the MCMC algorithm and should be
 * used when you want to effectively find the posterior distribution of 
 * variables.
 *
 * If instead your main focus it model comparison then you should use 
 * FPChainController, which is basically the same as this, but uses many more
 * mcmc chains to get a good approximation of the marginal likelihood of the
 * model.
 *
*/
class ChainController : public FPChainController {
	public:
		ChainController( const likelihood_t &loglikelihood, 
		const std::vector<parameter_t> &parameters,
		const joint_prior_t &jp, size_t warm_up, size_t total_steps,
			size_t no_threads, std::ostream &out = std::cout );

		ChainController( const likelihood_t &loglikelihood, 
		const std::vector<std::vector<parameter_t> > &pars_v,
		const joint_prior_t &jp, size_t warm_up, size_t total_steps,
			size_t no_threads, std::ostream &out = std::cout );
};
};
#endif
