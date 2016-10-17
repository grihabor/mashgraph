import os
import shutil
from sys import argv
from random import random

proj_dir = argv[1]
proj_dir = proj_dir[:proj_dir.rfind('/')]


# This is for previous script version to work
# START
if os.path.isdir('{}/data/base'.format(proj_dir)):
    if os.path.isdir('{}/data/multiclass'.format(proj_dir)):
        shutil.rmtree('{}/data/multiclass'.format(proj_dir))
    os.rename('{}/data/base'.format(proj_dir), '{}/data/multiclass'.format(proj_dir))
# END

# store original labels if not stored (first run)
if not os.path.exists('{}/data/labels.txt'.format(proj_dir)):
    shutil.copy('{}/data/multiclass/train_labels.txt'.format(proj_dir),
                '{}/data/labels.txt'.format(proj_dir))
if not os.path.isdir('{}/data/multiclass/test'.format(proj_dir)):
    shutil.copytree('{}/data/multiclass/train'.format(proj_dir),
                    '{}/data/multiclass/test'.format(proj_dir))

data = {}
dirs = {}
# load data
with open('{}/data/labels.txt'.format(proj_dir)) as base:
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
            for i, filename in enumerate(filenames):
                if random() < ratio:
                    train.write('train/{} {}\n'.format(filename, label))
                else:
                    test.write('test/{} {}\n'.format(filename, label))

print('Data generated')

