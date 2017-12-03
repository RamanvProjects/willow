from models.gan import Model, WGAN
from models.factories import gen_point_cloud, disc_point_cloud
import tensorflow as tf

flags = tf.app.flags
FLAGS = flags.FLAGS

flags.DEFINE_integer('num_epochs', 15, 'Number of epochs')
flags.DEFINE_integer('batch_size', 32, 'Batch size')
flags.DEFINE_integer('n_critic', 5, 'Number of critic updates per generator update')
flags.DEFINE_float('clip_weight', 1e-2, 'Clip weight for WGAN training')
flags.DEFINE_integer('latent_size', 100, 'Size of the latent vector')

G = Model(gen_point_cloud, [-1, 100], name='generator')
D = Model(disc_point_cloud, [-1, 3, 256], name='discriminator')
gan = WGAN(G, D, [-1, 3, 256], clip_weight=1e-2)

n_critic = FLAGS.n_critic
batch_size = FLAGS.batch_size

for i in range(FLAGS.num_epochs):
    for j in data.num_batches():
        for k in range(n_critic):
            batch_x = data.next_batch()
            loss_d = gan.partial_fit_discriminator(batch_x)

        loss_g = gan.partial_fit_generator(batch_size)
    