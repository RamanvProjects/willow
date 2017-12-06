from __future__ import division
from tflearn.layers.core import *
import tensorflow as tf
import numpy as np
import os
import utils


class SketchEncoder(object):
    def __init__(self, gan, logging, batch_size, learning_rate, input_shape, sess=None, model_path="checkpoints/models", name="sketch_encoder"):
        """
        Evaluation network for chess boards. Based on an MNIST CNN but for 8x8 images
        """
        # Network Parameters
        self.name = name
        self.model_path = model_path
        self.logging = logging
        self.batch_size = batch_size
        self.learning_rate = learning_rate
        self.input_shape = input_shape
        self.gan = gan
        self.vertex_shape = self.gan.discriminator.input_shape
        self.latent_size = self.gan.latent_size

        # Placeholders for inputs
        self.x = tf.placeholder(tf.float32, shape=self.input_shape)
        self.y = tf.placeholder(tf.float32, shape=[None, self.latent_size])
        self.x_vertices = tf.placeholder(tf.float32, shape=self.vertex_shape)
        self.keep_prob = tf.placeholder(tf.float32)

        # Set up weights, network and training logits
        self.logits = self._inference_graph()
        self.loss = self._loss()
        self.optimizer = self._optimizer()

        # Setting up instance
        init = tf.initialize_all_variables()
        self.saver = tf.train.Saver()
        self.sess = tf.Session() if sess is None else sess
        self.sess.run(init)

    def _inference_graph(self):
        with tf.variable_scope("inference") as scope:
            self.fc1_1 = fully_connected(self.x[:, 0, :], 256, activation='leaky_relu')
            self.fc1_2 = fully_connected(self.x[:, 1, :], 256, activation='leaky_relu')
            self.fc1_3 = fully_connected(self.x[:, 1, :], 256, activation='leaky_relu')

            self.d_1 = dropout(self.fc1_1, keep_prob=self.keep_prob)
            self.d_2 = dropout(self.fc1_2, keep_prob=self.keep_prob)
            self.d_3 = dropout(self.fc1_3, keep_prob=self.keep_prob)

            self.fc2_1 = fully_connected(self.d_1, 128, activation='leaky_relu')
            self.fc2_2 = fully_connected(self.d_2, 128, activation='leaky_relu')
            self.fc2_3 = fully_connected(self.d_3, 128, activation='leaky_relu')

            self.fc3_1 = fully_connected(self.fc2_1, 64, activation='leaky_relu')
            self.fc3_2 = fully_connected(self.fc2_2, 64, activation='leaky_relu')
            self.fc3_3 = fully_connected(self.fc2_3, 64, activation='leaky_relu')

            self.fc4_1 = fully_connected(self.fc3_1, self.latent_size, activation='linear')
            self.fc4_2 = fully_connected(self.fc3_2, self.latent_size, activation='linear') 
            self.fc4_3 = fully_connected(self.fc3_3, self.latent_size, activation='linear')

            self.sum = self.fc4_1 + self.fc4_2 + self.fc4_3

            return self.sum

    def _loss(self):
        # TODO: Include L2 regularization later
        self.gan.generator.add_placeholder("encoder_training", self.logits, front=self.x)
        self.projected_encoding = self.gan.generator.placeholders["encoder_training"][1]

        return tf.reduce_sum(tf.nn.l2_loss(self.projected_encoding - self.x_vertices))
    
    def _optimizer(self):
        return tf.train.AdamOptimizer(self.learning_rate).minimize(self.loss)

    def partial_fit_step(self, X, Y):
        """
        Train network on batch input X and Y, with X hog features of sketches and Y associated real vertices.
        """
        cost, _ = self.sess.run([self.loss, self.optimizer],
                                feed_dict={
                                    self.x: X,
                                    self.x_vertices: Y,
                                    self.keep_prob: 0.8
                                })
        
        return cost

    def encode(self, X):
        """
        Predicts either single X or batch, returns probability of white and black winning
        """

        return = self.sess.run(self.logits, feed_dict={self.x: X, self.keep_prob: 1.0})

    def save_model(self, directory=None):
        if directory is None:
            directory = self.model_path

        filename = "%s_%s" % (self.name, self.now().strftime("%Y-%m-%d-%H%M"))
        if not tf.gfile.Exists(directory):
            tf.gfile.MakeDirs(directory)
        
        self.saver.save(self.sess, join(directory, filename))