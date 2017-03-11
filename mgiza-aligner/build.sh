#!/bin/sh

make CORPUS_NAME=setimes_enhr SRC_LANG=en TRG_LANG=hr
make CORPUS_NAME=setimes_enhr SRC_LANG=en TRG_LANG=hr clean-intermediate-files

make CORPUS_NAME=europarljrc_enpl SRC_LANG=en TRG_LANG=pl
make CORPUS_NAME=europarljrc_enpl SRC_LANG=en TRG_LANG=pl clean-intermediate-files

make CORPUS_NAME=jrc200k_plen SRC_LANG=pl TRG_LANG=en
make CORPUS_NAME=jrc200k_plen SRC_LANG=pl TRG_LANG=en clean-intermediate-files
