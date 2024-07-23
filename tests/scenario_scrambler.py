import sys
import pathlib
import random

dir = sys.argv[1]
amount = int(sys.argv[2])

files = []
total_num_scenarios = 0

for filepath in pathlib.Path(dir).glob("*.scen"):
    files.append(filepath)
    total_num_scenarios += sum(1 for _ in open(filepath)) - 1

scenarios_so_far = 0

print("version 1")
while True:
    for filepath in files:
        with open(filepath) as f:
            for i, line in enumerate(f):
                if i != 0:
                    choose = random.choices([True, False], [amount, total_num_scenarios])[0]
                    if choose:
                        sys.stdout.write(line)
                        scenarios_so_far += 1
                        
                        if(scenarios_so_far == amount):
                            sys.exit()