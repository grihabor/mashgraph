from sys import argv

if len(argv) < 3:
    print("Usage: python average.py FILENAME STRING...")


values = []

with open(argv[1]) as f:
    for line in f:
        values.append(float(line))
        
print('Average {}: {}'.format(' '.join(argv[2:]), 
    sum(values)/len(values)))
