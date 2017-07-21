#!/usr/bin/python3
# -*- coding: utf-8 -*-

import sys

index = 0
with open(sys.argv[1]) as corpus:
    for line in corpus:
        index += 1
        print(str(index)+"\t"+line.rstrip())
