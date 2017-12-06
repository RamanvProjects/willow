from data import Data
from models.gan import Model, WGAN
from models.factories import gen_point_cloud, disc_point_cloud
from preprocess import parse_dir
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
flags.DEFINE_integer('save_step', 50, 'Save every (blank) batches')
flags.DEFINE_integer('summary_step', 20, 'Summarize every (blank) batches')
flags.DEFINE_integer('print_every', 10, 'Print every (blank) batches')
flags.DEFINE_string('data_dir', '../data/json', 'Directory containing data files, one tree per file')
flags.DEFINE_string('gan_file', None, 'Trained GAN checkpoint directory (absolutely required)')

if FLAGS.gan_file == None or not tf.gfile.Exists(FLAGS.gan_file):
    logger.warn("GAN directory must be provided and valid, exiting...")

batch_size = FLAGS.batch_size

# Waiting for data
points_per_tree = parse_dir(FLAGS.data_dir)
data = Data(points_per_tree, FLAGS.batch_size, logger=logger)
G = Model(gen_point_cloud, [None, 100], name='generator')
D = Model(disc_point_cloud, [None, 784], name='discriminator')
gan = WGAN(G, D, clip_weight=FLAGS.clip_weight, sess)

for epoch in tqdm(range(FLAGS.num_epochs), desc="Epoch"):
    for batch in tqdm(range(data.train.num_examples/FLAGS.batch_size), desc="Batch"):

        if batch % FLAGS.print_every == 0:
            logger.info("Epoch %d, Batch %d" % (epoch, batch))
            logger.info("Discriminator loss: %f" % (loss_d/float(FLAGS.n_critic)))
            logger.info("Generator loss: %f" % loss_g)
            k = gan.generate(n=1)
            print k
            img = Image.fromarray(np.reshape(k[0]*255, [28, 28]))
            img = img.convert('RGB')
            img.save('%s.png' % batch)