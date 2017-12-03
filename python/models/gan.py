""" A generic GAN training device

Author(s): Vivek Ramanujan, (Pato)
"""
import tensorflow as tf
import numpy as np

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

            return self.placeholders[name][1s]


class WGAN(object):
    def __init__(self, G, D, data_shape, clip_weight=1.0):
        self.generator = G
        self.latent_size = self.generator.input_shape[1]
        self.discriminator = D

        # Preparing the logits
        self.data_shape = data_shape
        self.clip_weight = clip_weight

        self._init_logits()
        self._init_weights()

        init = tf.initialize_all_variables()
        self.sess = tf.Session()
        self.sess.run(init)

    def _init_logits(self):
        self.discriminator.add_placeholder("fake", self.generator.default_logits)

        real_placeholder = tf.placeholder(dtype=tf.float32, shape=self.discriminator.input_shape, name="pato")
        self.discriminator.add_placeholder("real", real_placeholder)

    def _init_weights(self):
        self.discriminator_weights = 
                tf.get_collection(tf.GraphKeys.TRAINABLE_VARIABLES,
                                  scope=self.discriminator.name)

    def _loss(self):
        latent_input, fake_D = self.discriminator.placeholders["fake"]
        real_input, real_D = self.discriminator.placeholders["real"]

        self.gen_loss = tf.reduce_mean(fake_D)
        self.disc_loss = tf.reduce_mean(real_D) - tf.reduce_mean(fake_D)
        self.real_input = real_input
        self.latent_input = latent_input
        
    def _train_operations(self):
        self.clip = [weight.assign(tf.clip_by_value(weight, -self.clip_weight, self.clip_weight)) for weight in self.discriminator_weights]

        # TODO(vramanuj): Update separately
        # TODO(vramanuj): Weight clipping
        self.disc_train_op = tf.train.RMSPropOptimizer().minimize(self.disc_loss)
        self.gen_train_op = tf.train.RMSPropOptimizer().minimize(self.gen_loss)

    def partial_fit_discriminator(self, X):
        noise = np.random.rand(len(X), self.latent_size)
        loss, _ = self.sess.run([self.disc_loss, self.disc_train_op],
                                feed_dict={
                                    self.latent_input: noise,
                                    self.real_input: X
                                })
        
        return loss
    
    def partial_fit_generator(self, n_batch):
        noise = np.random.rand(n_batch, self.latent_size)
        loss, _ = self.sess.run([self.gen_loss, self.gen_train_op],
                                feed_dict={
                                    self.latent_input: noise,
                                })
        
        return loss
        
    def generate(self, z=None, n=1):
        if z is None:
            z = np.random.rand(n, self.latent_size)

        return self.sess.run(self.generator, feed_dict={self.latent_input: z})
