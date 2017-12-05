""" Neural network generation factories for use with generic training operations

Author: Vivek Ramanujan, (Pato)
"""
from tflearn.activations import leaky_relu
from tflearn.layers.core import *
from tflearn.layers.conv import *
import tensorflow as tf

tc = tf.contrib
tcl = tf.contrib.layers

def gen_point_cloud(input):
    fc1_1 = fully_connected(input, 64, activation='relu')
    fc1_2 = fully_connected(input, 64, activation='relu')
    fc1_3 = fully_connected(input, 64, activation='relu')

    fc2_1 = fully_connected(fc1_1, 128, activation='relu')
    fc2_2 = fully_connected(fc1_2, 128, activation='relu')
    fc2_3 = fully_connected(fc1_3, 128, activation='relu')

    fc3_1 = fully_connected(fc2_1, 256, activation='relu')
    fc3_2 = fully_connected(fc2_2, 256, activation='relu')
    fc3_3 = fully_connected(fc2_3, 256, activation='relu')

    # concat = tf.concat([fc3_1, fc3_2, fc3_3], axis=2)
    concat = tf.stack([fc3_1, fc3_2, fc3_3], axis=2)

    return concat

def disc_point_cloud(input):
    print "SHAPE INPUT: ", input.shape
    # leaky_relu = lambda x: leaky_relu(x, alpha=0.2, name="LeakyReLu")
    leaky_relu_lam = lambda x: leaky_relu(x, alpha=0.2, name="LeakyReLu")
    # fc3_1 = fully_connected(input[:, 0, :], 256, activation=leaky_relu_lam)
    # fc3_2 = fully_connected(input[:, 1, :], 256, activation=leaky_relu_lam)
    # fc3_3 = fully_connected(input[:, 2, :], 256, activation=leaky_relu_lam) 
    
    fc3_1 = fully_connected(input[:, :, 0], 256, activation=leaky_relu_lam)
    fc3_2 = fully_connected(input[:, :, 1], 256, activation=leaky_relu_lam)
    fc3_3 = fully_connected(input[:, :, 2], 256, activation=leaky_relu_lam) 

    fc2_1 = fully_connected(fc3_1, 128, activation=leaky_relu_lam)
    fc2_2 = fully_connected(fc3_2, 128, activation=leaky_relu_lam)
    fc2_3 = fully_connected(fc3_3, 128, activation=leaky_relu_lam)

    fc1_1 = fully_connected(fc2_1, 64, activation=leaky_relu_lam)
    fc1_2 = fully_connected(fc2_2, 64, activation=leaky_relu_lam)
    fc1_3 = fully_connected(fc2_3, 64, activation=leaky_relu_lam)

    concat = tf.concat([fc1_1, fc1_2, fc1_3], axis=1)
    reshaped = tf.reshape(concat, [-1, 64*3])
    fc_out = fully_connected(reshaped, 1)

    return tf.sigmoid(fc_out)

def mnist_gen(z):
    bs = tf.shape(z)[0]
    fc1 = tc.layers.fully_connected(
        z, 1024,
        weights_initializer=tf.random_normal_initializer(stddev=0.02),
        weights_regularizer=tc.layers.l2_regularizer(2.5e-5),
        activation_fn=tf.identity
    )
    fc1 = tc.layers.batch_norm(fc1)
    fc1 = tf.nn.relu(fc1)
    fc2 = tc.layers.fully_connected(
        fc1, 7 * 7 * 128,
        weights_initializer=tf.random_normal_initializer(stddev=0.02),
        weights_regularizer=tc.layers.l2_regularizer(2.5e-5),
        activation_fn=tf.identity
    )
    fc2 = tf.reshape(fc2, tf.stack([bs, 7, 7, 128]))
    fc2 = tc.layers.batch_norm(fc2)
    fc2 = tf.nn.relu(fc2)
    conv1 = tc.layers.convolution2d_transpose(
        fc2, 64, [4, 4], [2, 2],
        weights_initializer=tf.random_normal_initializer(stddev=0.02),
        weights_regularizer=tc.layers.l2_regularizer(2.5e-5),
        activation_fn=tf.identity
    )
    conv1 = tc.layers.batch_norm(conv1)
    conv1 = tf.nn.relu(conv1)
    conv2 = tc.layers.convolution2d_transpose(
        conv1, 1, [4, 4], [2, 2],
        weights_initializer=tf.random_normal_initializer(stddev=0.02),
        weights_regularizer=tc.layers.l2_regularizer(2.5e-5),
        activation_fn=tf.sigmoid
    )
    conv2 = tf.reshape(conv2, tf.stack([bs, 784]))
    return conv2

def mnist_disc(x):
    bs = tf.shape(x)[0]
    x = tf.reshape(x, [bs, 28, 28, 1])
    conv1 = tc.layers.convolution2d(
        x, 64, [4, 4], [2, 2],
        weights_initializer=tf.random_normal_initializer(stddev=0.02),
        activation_fn=tf.identity
    )
    conv1 = leaky_relu(conv1)
    conv2 = tc.layers.convolution2d(
        conv1, 128, [4, 4], [2, 2],
        weights_initializer=tf.random_normal_initializer(stddev=0.02),
        activation_fn=tf.identity
    )
    conv2 = leaky_relu(conv2)
    conv2 = tcl.flatten(conv2)
    fc1 = tc.layers.fully_connected(
        conv2, 1024,
        weights_initializer=tf.random_normal_initializer(stddev=0.02),
        activation_fn=tf.identity
    )
    fc1 = leaky_relu(fc1)
    fc2 = tc.layers.fully_connected(fc1, 1, activation_fn=tf.identity)

    return fc2