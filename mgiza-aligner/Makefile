SRC_LANG=en
TRG_LANG=pl
CORPUS_NAME=europarl_sample

all: corpora/$(CORPUS_NAME)/aligned.txt corpora/$(CORPUS_NAME)/src.tok corpora/$(CORPUS_NAME)/trg.tok

corpora/$(CORPUS_NAME)/aligned.txt: corpora/$(CORPUS_NAME)/giza.cfg corpora/$(CORPUS_NAME)/src.low_trg.low.cooc corpora/$(CORPUS_NAME)/src.low_trg.low.snt corpora/$(CORPUS_NAME)/src.low.vcb corpora/$(CORPUS_NAME)/trg.low.vcb
	mgiza/mgizapp/bin/mgiza corpora/$(CORPUS_NAME)/giza.cfg
	cat corpora/$(CORPUS_NAME)/aligned*part* | ./sortGizaAlignments.py > corpora/$(CORPUS_NAME)/aligned.txt

clean-intermediate-files:
	rm -f corpora/$(CORPUS_NAME)/*.lem
	rm -f corpora/$(CORPUS_NAME)/*.low
	rm -f corpora/$(CORPUS_NAME)/*.classes
	rm -f corpora/$(CORPUS_NAME)/*.classes.cats
	rm -f corpora/$(CORPUS_NAME)/*.vcb
	rm -f corpora/$(CORPUS_NAME)/*.snt
	rm -f corpora/$(CORPUS_NAME)/*.cooc
	rm -f corpora/$(CORPUS_NAME)/aligned*part*
	rm -f corpora/$(CORPUS_NAME)/giza.cfg


clean:
	rm -f corpora/$(CORPUS_NAME)/*.tok
	rm -f corpora/$(CORPUS_NAME)/*.lem
	rm -f corpora/$(CORPUS_NAME)/*.low
	rm -f corpora/$(CORPUS_NAME)/*.classes
	rm -f corpora/$(CORPUS_NAME)/*.classes.cats
	rm -f corpora/$(CORPUS_NAME)/*.vcb
	rm -f corpora/$(CORPUS_NAME)/*.snt
	rm -f corpora/$(CORPUS_NAME)/*.cooc
	rm -f corpora/$(CORPUS_NAME)/aligned*
	rm -f corpora/$(CORPUS_NAME)/giza.cfg

corpora/$(CORPUS_NAME)/giza.cfg: giza.cfg.pattern
	sed 's/CORPUS_NAME/'$(CORPUS_NAME)'/' < $< > $@

corpora/$(CORPUS_NAME)/src.low_trg.low.cooc: corpora/$(CORPUS_NAME)/src.low.vcb corpora/$(CORPUS_NAME)/trg.low.vcb corpora/$(CORPUS_NAME)/src.low_trg.low.snt
	mgiza/mgizapp/bin/snt2cooc $@ corpora/$(CORPUS_NAME)/src.low.vcb corpora/$(CORPUS_NAME)/trg.low.vcb corpora/$(CORPUS_NAME)/src.low_trg.low.snt

corpora/$(CORPUS_NAME)/src.low_trg.low.snt corpora/$(CORPUS_NAME)/trg.low_src.low.snt corpora/$(CORPUS_NAME)/src.low.vcb corpora/$(CORPUS_NAME)/trg.low.vcb: corpora/$(CORPUS_NAME)/src.low corpora/$(CORPUS_NAME)/trg.low
	mgiza/mgizapp/bin/plain2snt corpora/$(CORPUS_NAME)/src.low corpora/$(CORPUS_NAME)/trg.low

corpora/$(CORPUS_NAME)/%.classes: corpora/$(CORPUS_NAME)/%.low
	mgiza/mgizapp/bin/mkcls -n10 -p$< -V$@

corpora/$(CORPUS_NAME)/%.low: corpora/$(CORPUS_NAME)/%.lem
	tr '[:upper:]' '[:lower:]' < $< > $@

corpora/$(CORPUS_NAME)/trg.lem: corpora/$(CORPUS_NAME)/trg.tok
	mono LemmaGenSentenceLemmatizer/LemmaGenSentenceLemmatizer/bin/Debug/LemmaGenSentenceLemmatizer.exe $(TRG_LANG) < $< > $@


corpora/$(CORPUS_NAME)/src.lem: corpora/$(CORPUS_NAME)/src.tok
	mono LemmaGenSentenceLemmatizer/LemmaGenSentenceLemmatizer/bin/Debug/LemmaGenSentenceLemmatizer.exe $(SRC_LANG) < $< > $@

corpora/$(CORPUS_NAME)/src.tok corpora/$(CORPUS_NAME)/trg.tok: corpora/$(CORPUS_NAME)/src.txt corpora/$(CORPUS_NAME)/trg.txt
	europarl/tools/tokenizer.perl -l $(SRC_LANG) < corpora/$(CORPUS_NAME)/src.txt > corpora/$(CORPUS_NAME)/$(CORPUS_NAME).$(SRC_LANG)
	europarl/tools/tokenizer.perl -l $(TRG_LANG) < corpora/$(CORPUS_NAME)/trg.txt > corpora/$(CORPUS_NAME)/$(CORPUS_NAME).$(TRG_LANG)
	./clean-corpus-n.perl corpora/$(CORPUS_NAME)/$(CORPUS_NAME) $(TRG_LANG) $(SRC_LANG) corpora/$(CORPUS_NAME)/$(CORPUS_NAME)_clean 0 100
	mv corpora/$(CORPUS_NAME)/$(CORPUS_NAME)_clean.$(SRC_LANG) corpora/$(CORPUS_NAME)/src.tok
	mv corpora/$(CORPUS_NAME)/$(CORPUS_NAME)_clean.$(TRG_LANG) corpora/$(CORPUS_NAME)/trg.tok
	rm corpora/$(CORPUS_NAME)/$(CORPUS_NAME).$(SRC_LANG)
	rm corpora/$(CORPUS_NAME)/$(CORPUS_NAME).$(TRG_LANG)
