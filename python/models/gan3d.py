from tflearn.activations import leaky_relu
from tflearn.layers.conv import conv_3d, conv_3d_transpose
from tflearn.layers.core import *
import tensorflow as tf
import tflearn as tl


class Discriminator3d(object):
    def __init__(self, logging, batch_size, learning_rate, image_size, hidden_size):
        """
        Evaluation network for chess boards. Based on an MNIST CNN but for 8x8 images
        """
        # Network Parameters
        self.logging = logging
        self.batch_size = batch_size
        self.learning_rate = learning_rate
        self.image_size, self.hidden_size = image_size, hidden_size
        
        # Placeholders for inputs
        self.input_shape = [self.batch_size, self.image_size,\
                            self.image_size, self.image_size, 1]
        self.x = tf.placeholder(tf.float32, shape=self.input_shape)
        self.y = tf.placeholder(tf.float32, shape=[self.batch_size, 1])
        # self.y = tf.placeholder(tf.float32, shape=[1, self.batch_size])
        self.keep_prob = tf.placeholder(tf.float32)

        # Set up weights, network and training logits
        self.logits = self._inference_graph()
        self.loss = self._loss()
        self.optimizer = self._optimizer()

        # Setting up instance
        init = tf.initialize_all_variables()
        self.sess = tf.Session()
        self.sess.run(init)

    def _inference_graph(self):
        with tf.name_scope('discriminator'):
            self.conv1 = conv_3d(self.x, 1, [4]*3, activation='leaky_relu', strides=2)
            self.conv2 = conv_3d(self.conv1, 64, [4]*3, activation='leaky_relu', strides=2)
            self.conv3 = conv_3d(self.conv2, 128, [4]*3, activation='leaky_relu', strides=2)
            self.conv4 = conv_3d(self.conv3, 256, [4]*3, activation='leaky_relu', strides=2)
            self.conv5 = conv_3d(self.conv4, 512, [4]*3, activation='leaky_relu', strides=2)

        reshaped = reshape(self.conv5, [self.batch_size, -1, 1])
        self.out = fully_connected(reshaped, 1)

        return self.out
    
    def _loss(self):
        # TODO: Include L2 regularization later
        # TODO: Add policy gradient buffer updates with discounted rewards
        return tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits(logits=self.logits, labels=self.y))
    
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
