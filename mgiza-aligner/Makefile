SRC_LANG=en
TRG_LANG=hr
CORPUS_NAME=tmrepository_enhr
SEPARATOR=@\#@
DICTIONARY_WEIGHT=5

all: corpora/$(CORPUS_NAME)/aligned_final.txt corpora/$(CORPUS_NAME)/src_final.txt corpora/$(CORPUS_NAME)/trg_final.txt

corpora/$(CORPUS_NAME)/aligned.txt: corpora/$(CORPUS_NAME)/giza.cfg corpora/$(CORPUS_NAME)/src.lem_trg.lem.cooc corpora/$(CORPUS_NAME)/src.lem_trg.lem.snt corpora/$(CORPUS_NAME)/src.lem.vcb corpora/$(CORPUS_NAME)/trg.lem.vcb
	mgiza/mgizapp/bin/mgiza corpora/$(CORPUS_NAME)/giza.cfg
	cat corpora/$(CORPUS_NAME)/aligned*part* | ./sortGizaAlignments.py > corpora/$(CORPUS_NAME)/aligned.txt

clean-intermediate-files:
	rm -f corpora/$(CORPUS_NAME)/*.lem
	rm -f corpora/$(CORPUS_NAME)/*.tok
	rm -f corpora/$(CORPUS_NAME)/*.dict
	rm -f corpora/$(CORPUS_NAME)/*.classes
	rm -f corpora/$(CORPUS_NAME)/*.classes.cats
	rm -f corpora/$(CORPUS_NAME)/*.vcb
	rm -f corpora/$(CORPUS_NAME)/*.snt
	rm -f corpora/$(CORPUS_NAME)/*.cooc
	rm -f corpora/$(CORPUS_NAME)/aligned*part*
	rm -f corpora/$(CORPUS_NAME)/aligned.txt
	rm -f corpora/$(CORPUS_NAME)/giza.cfg
	rm -f corpora/$(CORPUS_NAME)/aligned.gizacfg
	rm -f corpora/$(CORPUS_NAME)/pasted.txt
	rm -f corpora/$(CORPUS_NAME)/pasted_deduplicated.txt
	rm -f corpora/$(CORPUS_NAME)/src_clean.txt
	rm -f corpora/$(CORPUS_NAME)/trg_clean.txt

clean: clean-intermediate-files
	rm -f corpora/$(CORPUS_NAME)/src_final.txt
	rm -f corpora/$(CORPUS_NAME)/trg_final.txt
	rm -f corpora/$(CORPUS_NAME)/aligned_final.txt

corpora/$(CORPUS_NAME)/giza.cfg: giza.cfg.pattern
	sed 's/CORPUS_NAME/'$(CORPUS_NAME)'/' < $< > $@

corpora/$(CORPUS_NAME)/src.lem_trg.lem.cooc: corpora/$(CORPUS_NAME)/src.lem.vcb corpora/$(CORPUS_NAME)/trg.lem.vcb corpora/$(CORPUS_NAME)/src.lem_trg.lem.snt
	mgiza/mgizapp/bin/snt2cooc $@ corpora/$(CORPUS_NAME)/src.lem.vcb corpora/$(CORPUS_NAME)/trg.lem.vcb corpora/$(CORPUS_NAME)/src.lem_trg.lem.snt

corpora/$(CORPUS_NAME)/src.lem_trg.lem.snt corpora/$(CORPUS_NAME)/trg.lem_src.lem.snt corpora/$(CORPUS_NAME)/src.lem.vcb corpora/$(CORPUS_NAME)/trg.lem.vcb: corpora/$(CORPUS_NAME)/src.lem corpora/$(CORPUS_NAME)/trg.lem
	mgiza/mgizapp/bin/plain2snt corpora/$(CORPUS_NAME)/src.lem corpora/$(CORPUS_NAME)/trg.lem

corpora/$(CORPUS_NAME)/%.classes: corpora/$(CORPUS_NAME)/%.lem
	mgiza/mgizapp/bin/mkcls -n10 -p$< -V$@

corpora/$(CORPUS_NAME)/trg.lem: corpora/$(CORPUS_NAME)/trg_clean.lem corpora/$(CORPUS_NAME)/trg.dict
	cat corpora/$(CORPUS_NAME)/trg_clean.lem corpora/$(CORPUS_NAME)/trg.dict > $@

corpora/$(CORPUS_NAME)/src.lem: corpora/$(CORPUS_NAME)/src_clean.lem corpora/$(CORPUS_NAME)/src.dict
	cat corpora/$(CORPUS_NAME)/src_clean.lem corpora/$(CORPUS_NAME)/src.dict > $@

corpora/$(CORPUS_NAME)/src.dict:
	./collect_dict.py $(SRC_LANG) $(TRG_LANG) $(DICTIONARY_WEIGHT) > $@

corpora/$(CORPUS_NAME)/trg.dict:
	./collect_dict.py $(TRG_LANG) $(SRC_LANG) $(DICTIONARY_WEIGHT) > $@


corpora/$(CORPUS_NAME)/trg_clean.lem: corpora/$(CORPUS_NAME)/trg_clean.tok
	mono LemmaGenSentenceLemmatizer/LemmaGenSentenceLemmatizer/bin/Debug/LemmaGenSentenceLemmatizer.exe $(TRG_LANG) < $< > $@

corpora/$(CORPUS_NAME)/src_clean.lem: corpora/$(CORPUS_NAME)/src_clean.tok
	mono LemmaGenSentenceLemmatizer/LemmaGenSentenceLemmatizer/bin/Debug/LemmaGenSentenceLemmatizer.exe $(SRC_LANG) < $< > $@


corpora/$(CORPUS_NAME)/trg.tok: corpora/$(CORPUS_NAME)/trg.txt
	/usr/local/bin/concordia-sentence-tokenizer -c ../concordia.cfg < $< > $@


corpora/$(CORPUS_NAME)/src.tok: corpora/$(CORPUS_NAME)/src.txt
	/usr/local/bin/concordia-sentence-tokenizer -c ../concordia.cfg < $< > $@


corpora/$(CORPUS_NAME)/src_clean.txt corpora/$(CORPUS_NAME)/trg_clean.txt corpora/$(CORPUS_NAME)/src_clean.tok corpora/$(CORPUS_NAME)/trg_clean.tok: corpora/$(CORPUS_NAME)/src.txt corpora/$(CORPUS_NAME)/trg.txt corpora/$(CORPUS_NAME)/src.tok corpora/$(CORPUS_NAME)/trg.tok
	./clean_corpus.py corpora/$(CORPUS_NAME)/src.txt corpora/$(CORPUS_NAME)/trg.txt corpora/$(CORPUS_NAME)/src.tok corpora/$(CORPUS_NAME)/trg.tok corpora/$(CORPUS_NAME)/src_clean.txt corpora/$(CORPUS_NAME)/trg_clean.txt corpora/$(CORPUS_NAME)/src_clean.tok corpora/$(CORPUS_NAME)/trg_clean.tok $(SEPARATOR)

corpora/$(CORPUS_NAME)/pasted.txt: corpora/$(CORPUS_NAME)/src_clean.txt corpora/$(CORPUS_NAME)/trg_clean.txt
	./paste.py corpora/$(CORPUS_NAME)/src_clean.txt corpora/$(CORPUS_NAME)/trg_clean.txt $(SEPARATOR)> $@

corpora/$(CORPUS_NAME)/pasted_deduplicated.txt: corpora/$(CORPUS_NAME)/pasted.txt
	sort -k 1.13 $< | uniq -s 12 | sort > $@

corpora/$(CORPUS_NAME)/aligned_final.txt corpora/$(CORPUS_NAME)/src_final.txt corpora/$(CORPUS_NAME)/trg_final.txt: corpora/$(CORPUS_NAME)/pasted_deduplicated.txt corpora/$(CORPUS_NAME)/aligned.txt
	./extract.py $< corpora/$(CORPUS_NAME)/aligned.txt corpora/$(CORPUS_NAME)/aligned_final.txt corpora/$(CORPUS_NAME)/src_final.txt corpora/$(CORPUS_NAME)/trg_final.txt $(SEPARATOR)
