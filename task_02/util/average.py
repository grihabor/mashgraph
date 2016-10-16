from sys import argv

def mean(l):
    if len(l) == 0:
        return 0
    return sum(l)/len(l)

values = []
with open(argv[1]) as f:
    for line in f:
        values.append(float(line.split()[-1]))

print('Mean precision: {}'.format(mean(values)))
