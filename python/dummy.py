import numpy as np

def generate_data(n, size):
    data = []
    for i in range(n):
        data.append(np.random.rand(*size))
    
    return data