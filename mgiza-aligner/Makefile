SRC_LANG=en
TRG_LANG=pl
CORPUS_NAME=europarl

all: corpora/$(CORPUS_NAME)/giza.cfg corpora/$(CORPUS_NAME)/src.low_trg.low.cooc corpora/$(CORPUS_NAME)/src.low_trg.low.snt corpora/$(CORPUS_NAME)/src.low.vcb corpora/$(CORPUS_NAME)/trg.low.vcb
	mgiza/mgizapp/bin/mgiza corpora/$(CORPUS_NAME)/giza.cfg
	cat corpora/$(CORPUS_NAME)/aligned*part* > corpora/$(CORPUS_NAME)/aligned.txt

clean:
	rm -f corpora/$(CORPUS_NAME)/*.tok
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

corpora/$(CORPUS_NAME)/%.low: corpora/$(CORPUS_NAME)/%.tok
	tr '[:upper:]' '[:lower:]' < $< > $@

corpora/$(CORPUS_NAME)/src.tok: corpora/$(CORPUS_NAME)/src.txt
	europarl/tools/tokenizer.perl -l $(SRC_LANG) < $< > $@

corpora/$(CORPUS_NAME)/trg.tok: corpora/$(CORPUS_NAME)/trg.txt
	europarl/tools/tokenizer.perl -l $(TRG_LANG) < $< > $@
