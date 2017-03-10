#!/usr/bin/python3

import sys, re

examples_dict = {}
p = re.compile("# Sentence pair \((\d+)\)")

i = 0
for line in sys.stdin:
    line = line.strip()
    if i % 3 == 0:
        current_example = [line]
        m = p.match(line)
        if m:
            current_key = int(m.group(1))
        else:
            raise Exception("Wrong line: "+line)
    elif i % 3 == 1:
        current_example.append(line)
    else:
        current_example.append(line)
        examples_dict[current_key] = current_example
    i+=1

for key in sorted(examples_dict.keys()):
    print ('\n'.join(examples_dict[key]))
