from skimage.io import imread
from sys import argv
import numpy as np

img1 = imread(argv[1])
img2 = imread(argv[2])

print(np.sum(img1 - img2))
