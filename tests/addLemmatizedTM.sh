#!/bin/sh

CORPUS_NAME="europarl_sample"
SRC_LANG_ID=1
TRG_LANG_ID=2

./addAlignedLemmatizedTM.py $CORPUS_NAME ../mgiza-aligner/corpora/$CORPUS_NAME/src_final.txt $SRC_LANG_ID ../mgiza-aligner/corpora/$CORPUS_NAME/trg_final.txt $TRG_LANG_ID ../mgiza-aligner/corpora/$CORPUS_NAME/aligned_final.txt
