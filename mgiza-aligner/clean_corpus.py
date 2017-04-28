#!/usr/bin/python3
# -*- coding: utf-8 -*-

import sys

max_tokens = 100
max_ratio = 4.0

separator = sys.argv[8]

with open(sys.argv[1]) as pasted_file, open(sys.argv[2]) as src_deduplicated_tok, open(sys.argv[3]) as trg_deduplicated_tok, open(sys.argv[4], 'w') as src_clean, open(sys.argv[5], 'w') as trg_clean, open(sys.argv[6], 'w') as src_clean_tok, open(sys.argv[7], 'w') as trg_clean_tok:
    for line in pasted_file:
        src_line_orig, trg_line_orig = line.strip()[12:].split(separator)
        src_line_tok = src_deduplicated_tok.readline().strip()
        trg_line_tok = trg_deduplicated_tok.readline().strip()
        src_token_count = len(src_line_tok.split())
        trg_token_count = len(trg_line_tok.split())
        if (src_token_count > 0 and trg_token_count > 0 and src_token_count <= max_tokens and trg_token_count <= max_tokens):
            ratio = float(src_token_count/trg_token_count) if src_token_count > trg_token_count else float(trg_token_count/src_token_count)
            if (ratio <= max_ratio):
                src_clean.write(src_line_orig+"\n")
                trg_clean.write(trg_line_orig+"\n")
                src_clean_tok.write(src_line_tok+"\n")
                trg_clean_tok.write(trg_line_tok+"\n")
