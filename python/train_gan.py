from data import Data
from models.gan import Model, WGAN
from models.factories import gen_point_cloud, disc_point_cloud
from preprocess import parse_dir
from PIL import Image
from tqdm import tqdm
import logging
import sys
import tensorflow as tf
import numpy as np
import os

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
flags.DEFINE_string('data_dir', 'data/json', 'Directory containing data files, one tree per file')
flags.DEFINE_string('checkpoint_dir', 'checkpoints/', 'Directory to store checkpoints')

batch_size = FLAGS.batch_size

# Waiting for data
points_per_tree = parse_dir(FLAGS.data_dir)
data = Data(points_per_tree, FLAGS.batch_size, shuf=False, logger=logger)
G = Model(gen_point_cloud, [None, FLAGS.latent_size], name='generator')
D = Model(disc_point_cloud, [None, 256, 3], name='discriminator')
gan = WGAN(G, D, clip_weight=FLAGS.clip_weight)

for epoch in tqdm(range(FLAGS.num_epochs), desc="Epoch"):
    for batch in tqdm(range(data.num_batches()), desc="Batch"):
        summarize = batch % FLAGS.summary_step == 0
        loss_d = 0
        for critic_step in range(FLAGS.n_critic):
            batch_x = data.next_batch()
            loss_d += gan.partial_fit_discriminator(batch_x,
                summarize=summarize and (critic_step == FLAGS.n_critic - 1))

        loss_g = gan.partial_fit_generator(batch_size, summarize=summarize)
        if batch % FLAGS.print_every == 0:
            logger.info("Epoch %d, Batch %d" % (epoch, batch))
            logger.info("Discriminator loss: %f" % (loss_d/float(FLAGS.n_critic)))
            logger.info("Generator loss: %f" % loss_g)
            k = gan.generate(n=1)
            print "Gen shape: ", k.shape
        
        if batch % FLAGS.save_step == 0:
            gan.save_model(directory=os.path.abspath(os.path.join(FLAGS.checkpoint_dir, 'models')))
