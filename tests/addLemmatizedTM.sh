#!/bin/sh

CORPUS_NAME="europarljrc"
SRC_LANG_ID=2
TRG_LANG_ID=1

./addAlignedLemmatizedTM.py $CORPUS_NAME ../mgiza-aligner/corpora/$CORPUS_NAME/src.tok $SRC_LANG_ID ../mgiza-aligner/corpora/$CORPUS_NAME/trg.tok $TRG_LANG_ID ../mgiza-aligner/corpora/$CORPUS_NAME/aligned.txt
