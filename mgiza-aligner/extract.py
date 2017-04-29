#!/usr/bin/python3
# -*- coding: utf-8 -*-

import sys, re

separator = sys.argv[6]

pair_pattern = re.compile(r'^# Sentence pair \((\d+)\).*')

def getNextSentencePair(f):
    result = dict()
    first_line = f.readline()
    m = pair_pattern.match(first_line)
    if m:
        result['number'] = int(m.group(1))
    else:
        raise Exception("Could not read sentence pair number from line: %s" %first_line)
    lines = [first_line]
    lines.append(f.readline())
    lines.append(f.readline())
    result['lines'] = lines
    return result

with open(sys.argv[1]) as pasted_file, open(sys.argv[2]) as aligned_file, open(sys.argv[3], 'w') as aligned_final, open(sys.argv[4], 'w') as src_final, open(sys.argv[5], 'w') as trg_final:
    for line in pasted_file:
        src_line, trg_line = line.strip()[12:].split(separator)
        src_final.write(src_line+"\n")
        trg_final.write(trg_line+"\n")

        number = int(line[:12])
        found = False
        while not found:
            sentence_pair = getNextSentencePair(aligned_file)
            if (sentence_pair['number'] == number):
                found = True
        aligned_final.write(''.join(sentence_pair['lines']))
