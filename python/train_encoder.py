from data import Data
from models.gan import Model, WGAN
from models.sketch_encoder import SketchEncoder
from models.factories import gen_point_cloud, disc_point_cloud
from models.utils import import_sess
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
flags.DEFINE_integer('hog_size', 64, 'HOG feature size')
flags.DEFINE_string('model_dir', 'checkpoints/models', 'Model save directory')

if FLAGS.gan_file == None or not tf.gfile.Exists(FLAGS.gan_file):
    logger.warn("GAN directory must be provided and valid, exiting...")

sess = import_sess(FLAGS.gan_file)

# Waiting for data
points_per_tree = parse_sketches(FLAGS.data_dir)
data = Data(points_per_tree, FLAGS.batch_size, logger=logger)
G = Model(gen_point_cloud, [None, FLAGS.latent_size], name='generator')
D = Model(disc_point_cloud, [None, 256, 3], name='discriminator')
gan = WGAN(G, D, clip_weight=FLAGS.clip_weight, sess=sess)

encoder = SketchEncoder(gan, logging=logger, learning_rate=FLAGS.learning_rate, input_shape=[None, 3, FLAGS.hog_size], batch_size=FLAGS.batch_size, sess=sess, model_path=os.path.abspath(FLAGS.model_dir))


for epoch in tqdm(range(FLAGS.num_epochs), desc="Epoch"):
    for batch in tqdm(range(data.num_batches()):
        x, y = data.next_batch()
        batch_loss = encoder.partial_fit_step(x, y)

        if batch % FLAGS.print_every == 0:
            logger.info("Epoch %d, Batch %d" % (epoch, batch))
            logger.info("L2 loss: %f" % (batch_loss/float(FLAGS.batch_size)))
        
        if batch % FLAGS.save_every == 0:
            encoder.save_model()
