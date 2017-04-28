#!/usr/bin/python3
# -*- coding: utf-8 -*-

import sys

separator = sys.argv[3]

with open(sys.argv[1]) as src_file, open(sys.argv[2]) as trg_file:
    index = 0
    for src_line in src_file:
        trg_line = trg_file.readline()
        if separator in src_line or separator in trg_line:
            raise Exception("Can not use: "+separator+" as a separator. Please set a different one in the Makefile")
        print ("%012d%s%s%s" % (index, src_line.strip(), separator, trg_line.strip()))
        index += 1
