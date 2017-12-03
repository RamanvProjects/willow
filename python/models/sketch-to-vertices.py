from __future__ import division
from tflearn.layers.core import *
from tflearn.layers.conv import *
import tensorflow as tf
import numpy as np
import utils


FLAGS = tf.app.flags.FLAGS

class SketchToPointsFC(object):
    def __init__(self, logging, batch_size, learning_rate, image_size, sess=None):
        """
        Evaluation network for chess boards. Based on an MNIST CNN but for 8x8 images
        """
        # Network Parameters
        self.logging = FLAGS.logging
        self.batch_size = FLAGS.batch_size
        self.learning_rate = FLAGS.learning_rate
        self.image_size, self.hidden_size = FLAGS.image_size

        # Placeholders for inputs
        self.input_shape = [self.batch_size, self.image_size,\
                            self.image_size]
        self.x = tf.placeholder(tf.int32, shape=self.input_shape)
        self.y = tf.placeholder(tf.int8, shape=[self.batch_size])
        self.keep_prob = tf.placeholder(tf.float32)

        # Set up weights, network and training logits
        self.logits = self._inference_graph()
        self.loss = self._loss()
        self.optimizer = self._optimizer()

        # Setting up instance
        init = tf.initialize_all_variables()
        self.sess = tf.Session() if sess is None else sess
        self.sess.run(init)

    def _inference_graph(self, training):
        self.conv1 = conv_2d(self.x, 64, [15, 15, self.image_size], strides=3, activation='relu')
        self.max_pool_1 = max_pool_2d(self.conv1, kernel_size=[3, 3], strides=2, padding='same')

        self.conv2 = conv_2d(self.max_pool_1, 128, [5, 5, 64], strides=1, activation='relu')
        self.max_pool_2 = max_pool_2d(self.conv2, kernel_size=[3, 3], strides=2, padding='same')

        self.conv3 = conv_2d(self.max_pool_2, 256, [3, 3, 128], strides=1, activation='relu')
        self.conv4 = conv_2d(self.conv3, 256, [3, 3, 256], strides=1, activation='relu')

        self.conv5 = conv_2d(self.conv4, 256, [3, 3, 256], strides=1, activation='relu')
        self.max_pool_3 = max_pool_2d(self.conv5, kernel_size=[3, 3], strides=2, padding='same')

        self.conv6 = conv_2d(self.max_pool_3, 512, [7, 7, 256], strides=1, activation='relu')
        self.dropout_1 = dropout(self.conv6, keep_prob=self.keep_prob)

        self.conv7 = conv_2d(self.dropout_1, 512, [1, 1, 512], strides=1, activation='relu')
        self.dropout_2 = dropout(self.conv7, keep_prob=self.keep_prob)

        self.conv8 = conv_2d(self.dropout_2, 512, [1, 1, 512], strides=1, activation='linear')
        
        

    def _loss(self):
        # TODO: Include L2 regularization later
        # TODO: Add policy gradient buffer updates with discounted rewards
        return tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits(self.logits, self.y))
    
    def _optimizer(self):
        return tf.train.AdamOptimizer(self.learning_rate)

    def partial_fit_step(self, X, Y):
        """
        Train network on batch input X and Y
        """
        cost, _ = self.sess.run([self.loss, self.optimizer],
                                feed_dict={
                                    self.x: X,
                                    self.y: Y,
                                    self.keep_prob: 0.8
                                })
        
        return cost

    def predict(self, X):
        """
        Predicts either single X or batch, returns probability of white and black winning
        """

        prediction = self.sess.run(self.logits, feed_dict={self.x: X, self.keep_prob: 1.0})

        return prediction
