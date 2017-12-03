""" A generic GAN training device

Author(s): Vivek Ramanujan, (Pato)
"""
from datetime import datetime
from os.path import join
from utils import maybe_make_dir
import tensorflow as tf
import logging
import numpy as n

class Model(object):
    def __init__(self, nn_factory, input_shape, name):
        self.factory = nn_factory
        self.placeholders = {}
        self.name = name

        with tf.variable_scope(name) as scope:
            self.default_placeholder = tf.placeholder(dtype=tf.float32, shape=input_shape)
            self.default_logits = self.factory(self.default_placeholder)

        self.input_shape = input_shape

    def add_placeholder(self, name, logits):
        """ Sets placeholders and returns handles to placeholders (with names)
        """

        self._inference_graph(logits, name)

        return self.placeholders[name]

    def get_placeholders(self):
        """ Returns current placeholder dictionary
        """
        return self.placeholders
    
    def _inference_graph(self, input, name):
        with tf.variable_scope(self.name) as scope:
            scope.reuse_variables()
            self.placeholders[name] = (input, self.factory(input))

            return self.placeholders[name][1]


class WGAN(object):
    def __init__(self, G, D, data_shape, learning_rate=1e-5, clip_weight=1.0, logger=None, saver=None, summary_dir='checkpoints/summaries', name='wgan'):
        if logger is None:
            self.logger = logging.getLogger(__name__)
        else:
            self.logger = logger
        
        self.now = datetime.now
        self.step = 1

        # Setting up model parameters
        self.generator = G
        self.latent_size = self.generator.input_shape[1]
        self.discriminator = D
        self.lr = learning_rate
        self.name = name

        # Preparing the logits
        self.data_shape = data_shape
        self.clip_weight = clip_weight
        self._init_logits()
        self._init_weights()

        # Train ops
        self._loss()
        self._train_operations()

        # Set up saving and summaries
        self._init_summaries()
        self.saver = tf.train.Saver()

        train_dir = join(summary_dir, 'train')
        test_dir = join(summary_dir, 'train')
        maybe_make_dir(train_dir)
        maybe_make_dir(test_dir)

        self.train_writer = tf.summary.FileWriter(train_dir)
        self.test_writer = tf.summary.FileWriter(test_dir)

        init = tf.initialize_all_variables()
        self.sess = tf.Session()
        self.sess.run(init)

    def _init_logits(self):
        self.discriminator.add_placeholder("fake", self.generator.default_logits)

        print "SHAPE:", self.discriminator.input_shape
        real_placeholder = tf.placeholder(dtype=tf.float32, shape=self.discriminator.input_shape, name="pato")
        self.discriminator.add_placeholder("real", real_placeholder)

    def _init_weights(self):
        self.discriminator_weights = \
                tf.get_collection(tf.GraphKeys.TRAINABLE_VARIABLES,
                                  scope=self.discriminator.name)
        
        self.generator_weights = \
                tf.get_collection(tf.GraphKeys.TRAINABLE_VARIABLES,
                                  scope=self.generator.name)

    def _init_summaries(self):
        with tf.name_scope("summaries"):
            self.latent_sum = tf.summary.scalar("Latent Summary", self.latent_input)
            self.gen_sum = tf.summary.scalar("Generator Loss", self.gen_loss)
            self.gen_weight_sum = tf.summary.histogram("Generator Weight Summary", self.generator_weights)

            self.disc_sum = tf.summary.scalar("Discriminator Loss", self.disc_loss)
            self.disc_weight_sum = tf.summary.histogram("Discriminator Weight Summary", self.discriminator_weights)

    def _loss(self):
        self.latent_input, fake_D = self.discriminator.placeholders["fake"]
        self.real_input, real_D = self.discriminator.placeholders["real"]

        print 
        print self.discriminator.placeholders
        self.gen_loss = tf.reduce_mean(fake_D)
        self.disc_loss = tf.reduce_mean(fake_D - real_D)

        self.gen_summary_op = tf.summary.merge([self.latent_sum, self.gen_sum, self.gen_weight_sum])
        self.disc_summary_op = tf.summary.merge_all([self.latent_sum, self.disc_sum, self.disc_weight_sum])

    def _train_operations(self):
        self.clip = [weight.assign(tf.clip_by_value(weight, -self.clip_weight, self.clip_weight)) for weight in self.discriminator_weights]

        self.optimizer = tf.train.RMSPropOptimizer(self.lr)
        self.disc_train_op = self.optimizer.minimize(self.disc_loss, var_list=self.discriminator_weights)
        self.gen_train_op = self.optimizer.minimize(self.gen_loss, var_list=self.generator_weights)

    def partial_fit_discriminator(self, X, summarize=False):
        fetches = [self.disc_loss, self.disc_train_op]
        if summarize:
            fetches.append(self.disc_summary_op)

        noise = np.random.rand(len(X), self.latent_size)
        fetched = self.sess.run(fetches,
                                feed_dict={
                                    self.latent_input: noise,
                                    self.real_input: X
                                })
        self.sess.run(self.clip)
        self.step += 1

        if summarize:
            self.write_summary(fetched[2])

        return fetched[0]
    
    def partial_fit_generator(self, n_batch, summarize=False):
        fetches = [self.gen_loss, self.gen_train_op]
        if summarize:
            fetches.append(self.gen_summary_op)

        noise = np.random.rand(n_batch, self.latent_size)
        loss, _ = self.sess.run([self.gen_loss, self.gen_train_op],
                                feed_dict={
                                    self.latent_input: noise,
                                })
        self.step += 1

        if summarize:
            self.write_summary(fetched[2])

        return loss
        
    def generate(self, z=None, n=1):
        if z is None:
            z = np.random.rand(n, self.latent_size)

        return self.sess.run(self.generator, feed_dict={self.latent_input: z})
    
    def save_model(self, directory='checkpoints/models'):
        filename = "%s_%s" % (self.name, self.now().strftime("%Y-%m-%d-%H%M"))
        if not tf.gfile.Exists(directory):
            tf.gfile.MakeDirs(directory)
        
        self.saver.save(self.sess, join(directory, filename))

    def write_summary(self, summary, directory):
        self.writer.add_summary(summary, global_step=self.step)
