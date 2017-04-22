#!/bin/sh

make CORPUS_NAME=stocznia_plen SRC_LANG=pl TRG_LANG=en
make CORPUS_NAME=stocznia_plen SRC_LANG=pl TRG_LANG=en clean-intermediate-files

make CORPUS_NAME=stocznia_enpl SRC_LANG=en TRG_LANG=pl
make CORPUS_NAME=stocznia_enpl SRC_LANG=en TRG_LANG=pl clean-intermediate-files
