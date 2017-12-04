from data import Data
from models.gan import Model, WGAN
from models.factories import gen_point_cloud, disc_point_cloud, mnist_disc, mnist_gen
from PIL import Image
from tqdm import tqdm
import logging
import sys
import tensorflow as tf
import numpy as np

logger = logging.getLogger(__name__)
out_hdlr = logging.StreamHandler(sys.stdout)
out_hdlr.setFormatter(logging.Formatter('%(asctime)s %(message)s'))
out_hdlr.setLevel(logging.INFO)
logger.addHandler(out_hdlr)
logger.setLevel(logging.INFO)

flags = tf.app.flags
FLAGS = flags.FLAGS

flags.DEFINE_integer('num_epochs', 15, 'Number of epochs')
flags.DEFINE_integer('batch_size', 64, 'Batch size')
flags.DEFINE_integer('n_critic', 5, 'Number of critic updates per generator update')
flags.DEFINE_float('clip_weight', 5e-2, 'Clip weight for WGAN training')
flags.DEFINE_integer('latent_size', 100, 'Size of the latent vector')
flags.DEFINE_integer('save_step', 50, 'Save every (blank) batches')
flags.DEFINE_integer('summary_step', 20, 'Summarize every (blank) batches')
flags.DEFINE_integer('print_every', 10, 'Print every (blank) batches')

# G = Model(gen_point_cloud, [-1, 100], name='generator')
# D = Model(disc_point_cloud, [-1, 3, 256], name='discriminator')
# gan = WGAN(G, D, [-1, 3, 256], clip_weight=1e-2)

batch_size = FLAGS.batch_size

# Waiting for data
# data = Data([1, 2, 3], FLAGS.batch_size, logger=logger)
from tensorflow.examples.tutorials.mnist import input_data
data = input_data.read_data_sets('MNIST_data/')
G = Model(mnist_gen, [None, 100], name='generator')
D = Model(mnist_disc, [None, 784], name='discriminator')
gan = WGAN(G, D, clip_weight=FLAGS.clip_weight)

for epoch in tqdm(range(FLAGS.num_epochs), desc="Epoch"):
    for batch in tqdm(range(data.train.num_examples/FLAGS.batch_size), desc="Batch"):
        summarize = batch % FLAGS.summary_step == 0
        loss_d = 0
        for critic_step in range(FLAGS.n_critic):
            batch_x = data.train.next_batch(FLAGS.batch_size)
            loss_d += gan.partial_fit_discriminator(batch_x[0],
                summarize=summarize and (critic_step == FLAGS.n_critic - 1))

        loss_g = gan.partial_fit_generator(batch_size, summarize=summarize)
        if batch % FLAGS.print_every == 0:
            logger.info("Epoch %d, Batch %d" % (epoch, batch))
            logger.info("Discriminator loss: %f" % (loss_d/float(FLAGS.n_critic)))
            logger.info("Generator loss: %f" % loss_g)
            k = gan.generate(n=1)
            print k
            img = Image.fromarray(np.reshape(k[0]*255, [28, 28]))
            img = img.convert('RGB')
            img.save('%s.png' % batch)