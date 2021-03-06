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
#ifndef TEMPERATURE_HH 
#define TEMPERATURE_HH
#include <random>
#include <cppa/cppa.hpp>

#include "src/chainmcmc/trace.hh"

namespace chainmcmc {
	/**
	 * \brief Contains helper function for handling chains with different temperatures
	 */
	namespace temperature {
		using namespace cppa;

		class ChainState {
			public:
				double current_t;
				actor_ptr chain;
				actor_ptr logger;

				bool operator<( const ChainState &cs ); 
		};

		bool accept( std::mt19937 &engine, const ChainState &chainState1, 
				const ChainState &chainState2 );

		void swap( ChainState & cs1, ChainState & cs2 );
	};
};
#endif
	
