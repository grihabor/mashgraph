import os
import shutil
from sys import argv
from random import random

proj_dir = argv[1]
proj_dir = proj_dir[:proj_dir.rfind('/')]

if os.path.isdir('{}/data/multiclass'.format(proj_dir)):
    shutil.rmtree('{}/data/multiclass'.format(proj_dir))

os.mkdir('{}/data/multiclass'.format(proj_dir))
os.mkdir('{}/data/multiclass/train'.format(proj_dir))
os.mkdir('{}/data/multiclass/test'.format(proj_dir))

data = {}
dirs = {}
# load data
with open('{}/data/base/train_labels.txt'.format(proj_dir)) as base:
    for line in base:
        filename, label = line.strip().split()
        filename = filename[filename.find('/')+1:]
        dirs[label] = filename[:filename.find('/')]
        try:
            data[label].append(filename)
        except KeyError:
            data[label] = [filename]

ratio = float(argv[2])

with open('{}/data/multiclass/train_labels.txt'.format(proj_dir), 'w') as train:
    with open('{}/data/multiclass/test_labels.txt'.format(proj_dir), 'w') as test:
        for label, filenames in data.items():

            os.mkdir('{}/data/multiclass/train/{}'.format(proj_dir, dirs[label]))
            os.mkdir('{}/data/multiclass/test/{}'.format(proj_dir, dirs[label]))

            for i, filename in enumerate(filenames):
                if random() < ratio:
                    train.write('train/{} {}\n'.format(filename, label))
                    shutil.copy('{}/data/base/train/{}'.format(proj_dir, filename), 
                                '{}/data/multiclass/train/{}'.format(proj_dir, filename))
                else:
                    test.write('test/{} {}\n'.format(filename, label))
                    shutil.copy('{}/data/base/train/{}'.format(proj_dir, filename), 
                                '{}/data/multiclass/test/{}'.format(proj_dir, filename))

print('Data generated')

