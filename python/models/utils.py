from tensorflow import Variable, gfile, truncated_normal, constant
from models import SketchEncoder

from cv2 import HOGDescriptor, imread
from os import listdir, path

def maybe_make_dir(directory):
    if not gfile.Exists(directory):
        gfile.MakeDirs(directory)

def clear_dirs(directory):
    if gfile.Exists(directory):
        gfile.DeleteRecursively(directory)

def init_weight(shape, name=None):
    return Variable(truncated_normal(shape, stddev=0.1), name=name)

def init_bias(shape, value=0.1, name=None):
    return Variable(constant(value, shape=[shape]), name=name)


def parse_sketch_dir(dir_name, latent_size, ckpt_path):
	latent_vecs = []

	with tf.Session() as sess:
		saver.restore(sess, ckpt_path)
		fcn = SketchEncoder(sess=sess) # TODO
`
		for file_name in listdir(dir_name):
			full_path = path.join(dir_name, file_name)
			latent_vec = sketch_to_latent(fcn, full_path)
			latent_vecs.append(latent_vec)

def sketch_to_latent(fcn, img_path):
	hog_feat = compute_hog(img_path)
	return fcn.encode(hog_feat)


def compute_hog(img_path):
	img = imread(img_path)
	return hog.compute(im)