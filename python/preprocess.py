from collections import defaultdict
import json

test_json = '{"1,2,3":["1,2,3"], "4,6,5":["1,2,3","8,9,10"]}'

k = json.loads(test_json)
j = defaultdict(list)

for l, v in k.iteritems():
    l = tuple(map(float, l.split(',')))
    
    for q in v:
        a, b, c = map(float, q.split(','))
        j[l].append((a, b, c))

print j.keys()

