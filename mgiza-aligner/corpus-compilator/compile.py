#!/usr/bin/python3
# -*- coding: utf-8 -*-

import sys


with open(sys.argv[1]) as src_clean, open(sys.argv[2]) as trg_clean, open(sys.argv[3]) as corpus_lines, open(sys.argv[4], 'w') as src_filtered, open(sys.argv[5],'w') as trg_filtered:
    index = 1
    for corpus_line_raw in corpus_lines:
        corpus_line = int(corpus_line_raw)
        while index < corpus_line:
            src_clean.readline()
            trg_clean.readline()
            index+=1
        src_filtered.write(src_clean.readline())
        trg_filtered.write(trg_clean.readline())
        index+=1
