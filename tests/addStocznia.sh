#!/bin/sh

./addTm.py 1 2 placeholder 1

./addAlignedLemmatizedTM.py stocznia_plen ../mgiza-aligner/corpora/stocznia_plen/src_final.txt 1 ../mgiza-aligner/corpora/stocznia_plen/trg_final.txt 2 ../mgiza-aligner/corpora/stocznia_plen/aligned_final.txt

./addTm.py 1 2 placeholder 1

./addTm.py 1 2 placeholder 1

./addAlignedLemmatizedTM.py stocznia_enpl ../mgiza-aligner/corpora/stocznia_enpl/src_final.txt 2 ../mgiza-aligner/corpora/stocznia_enpl/trg_final.txt 1 ../mgiza-aligner/corpora/stocznia_enpl/aligned_final.txt

