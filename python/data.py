""" Generic data class for data processing

Author: Vivek Ramanujan, (Pato)
"""
from random import shuffle

class Data(object):
    def __init__(self, data, batch_size, shuf=True, logger=None):
        self.logger = logger
        self.data = data
        self.shuffle = shuf
        if self.shuffle:
            shuffle(self.data)

        self.size = len(data)
        self.batch_size = batch_size
        self.curr_idx = 0

    def num_batches(self):
        return self.size/self.batch_size
    
    def next_batch(self):
        if self.curr_idx + self.batch_size > self.size:
            self.logger.info("Data reset to index 0")
            self.curr_idx = 0
            if self.shuffle:
                shuffle(self.data)
        
        batch = self.data[self.curr_idx:self.curr_idx + self.batch_size]
        self.curr_idx += self.batch_size

        return batch
            
        



