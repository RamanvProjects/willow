""" Neural network generation factories for use with generic training operations

Author: Vivek Ramanujan, (Pato)
"""
from tflearn.activations import leaky_relu
from tflearn.layers.core import *
from tflearn.layers.conv import *
import tensorflow as tf

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

    concat = tf.concat([fc3_1, fc3_2, fc3_3], axis=2)

    return concat

def disc_point_cloud(input):
    leaky_relu = lambda x: leaky_relu(x, alpha=0.2, name="LeakyReLu")
    fc3_1 = fully_connected(input[:, 0, :], 256, activation=leaky_relu)
    fc3_2 = fully_connected(input[:, 1, :], 256, activation=leaky_relu)
    fc3_3 = fully_connected(input[:, 2, :], 256, activation=leaky_relu) 

    fc2_1 = fully_connected(fc3_1, 128, activation=leaky_relu)
    fc2_2 = fully_connected(fc3_2, 128, activation=leaky_relu)
    fc2_3 = fully_connected(fc3_3, 128, activation=leaky_relu)

    fc1_1 = fully_connected(fc2_1, 64, activation=leaky_relu)
    fc1_2 = fully_connected(fc2_2, 64, activation=leaky_relu)
    fc1_3 = fully_connected(fc2_3, 64, activation=leaky_relu)

    concat = tf.concat([fc1_1, fc1_2, fc1_3], axis=2)
    reshaped = tf.reshape(concat, [-1, 64*3])
    fc_out = fully_connected(reshaped, 1)

    return tf.sigmoid(fc_out)