#!/usr/bin/python3
# -*- coding: utf-8 -*-

import sys, os, bz2

src_lang = sys.argv[1]
trg_lang = sys.argv[2]
weight = int(sys.argv[3])

for dname in os.listdir('dictionaries'):
    src_path = 'dictionaries/%s/%s.bz2' % (dname, src_lang)
    trg_path = 'dictionaries/%s/%s.bz2' % (dname, trg_lang)
    if os.path.isfile(src_path) and os.path.isfile(trg_path):
        with bz2.open(src_path, 'rt') as src_dict_file:
            for line in src_dict_file:
                for i in range(weight):
                    print(line.strip())
