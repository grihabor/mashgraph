from sys import argv
import subprocess

if len(argv) < 3:
    print('Usage: python run.py FILENAME RUN_COUNT')
    sys.exit()
    

cmd_1 = "template/build/bin/task2 " + "{} {}".format(argv[1], argv[2])
cmd_2 = "template/build/bin/task2 --sse " + "{} {}".format(argv[1], argv[2])

output = [None, None]

process_1 = subprocess.Popen(cmd_1.split(), stdout=subprocess.PIPE)
output[0], error_1 = process_1.communicate()
process_2 = subprocess.Popen(cmd_2.split(), stdout=subprocess.PIPE)
output[1], error_2 = process_2.communicate()

time_count = []

print()
print(argv[1])
for text in output:
    text = text.decode('utf-8').split('\n')[1]
    time_count.append(float(text[text.find('[') + 1 : text.find(']')]))
    print(text)

speed_up = time_count[0]/time_count[1] if time_count[1] != 0 else None
print('Speed up: {}'.format(speed_up if speed_up else "+inf"))

with open('speed_up.txt', 'a') as f:
    if speed_up:
        f.write('{}\n'.format(speed_up))
