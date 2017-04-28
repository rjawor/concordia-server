#!/usr/bin/python3
# -*- coding: utf-8 -*-

import sys

separator = sys.argv[4]

with open(sys.argv[1]) as pasted_file, open(sys.argv[2], 'w') as src_file, open(sys.argv[3], 'w') as trg_file:
    for line in pasted_file:
        src_line, trg_line = line.strip()[12:].split(separator)
        src_file.write(src_line+"\n")
        trg_file.write(trg_line+"\n")
