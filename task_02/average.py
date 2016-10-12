from sys import argv
from statistics import mean

values = []
with open(argv[1]) as f:
    for line in f:
        values.append(float(line.split()[-1]))

print('Mean error: {}'.format(mean(values)))