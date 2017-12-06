""" To parse weird data is to become weird data
"""
from collections import defaultdict
from numpy import vstack, argsort
import json
from os import listdir
from os.path import join

# test_json = open('data/json/graph.00001.obj.json').read() # '{"1,2,3":["1,2,3"], "4,6,5":["1,2,3","8,9,10"]}'

def parse_dir(dir_name):
    parsed_trees = []
    for file_name in listdir(dir_name):
        print "Parsing file: ", file_name
        parsed_trees.append(parse_file(join(dir_name, file_name)))
    return parsed_trees

def parse_file(file_name):
    json_file = open(file_name).read()
    k = json.loads(json_file)
    j = defaultdict(list)

    for l, v in k.iteritems():
        l = tuple(map(float, l.split(',')))
        
        for q in v:
            a, b, c = map(float, q.split(','))
            j[l].append((a, b, c))

    verts = vstack(j.keys())
    sorted_verts = verts[argsort(verts[:, 1])]

    return sorted_verts



# Testing one file
# print parse_file('data/json/graph.00001.obj.json')

