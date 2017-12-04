from tensorflow import Variable, gfile, truncated_normal, constant

def maybe_make_dir(directory):
    if not gfile.Exists(directory):
        gfile.MakeDirs(directory)

def init_weight(shape, name=None):
    return Variable(truncated_normal(shape, stddev=0.1), name=name)

def init_bias(shape, value=0.1, name=None):
    return Variable(constant(value, shape=[shape]), name=name)