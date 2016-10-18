from sys import argv, exit

try:
    filename = argv[1]
except IndexError:
    print('Failed to get params')
    exit(0)

try:
    with open(filename) as f:
        for line in f:
            line = line.split()
            if len(line) < 4:
                continue

            if line[0] == 'int' and line[1].upper() == line[1]:
                print(line[1], '=', line[3][:-1])
except IOError as e:
    print(e)
