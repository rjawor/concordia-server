SRC_LANG=pl
TRG_LANG=en
CORPUS_NAME=europarl_sample
DICTIONARY_NAME=classyf_popular_medicine
SEPARATOR=@\#@
CORPUS_CHUNK_SIZE=100000

all: corpora/$(CORPUS_NAME)/src_filtered.txt corpora/$(CORPUS_NAME)/trg_filtered.txt

corpora/$(CORPUS_NAME)/src_filtered.txt corpora/$(CORPUS_NAME)/trg_filtered.txt: corpora/$(CORPUS_NAME)/src_clean.txt corpora/$(CORPUS_NAME)/trg_clean.txt corpora/$(CORPUS_NAME)/corpus_lines.txt
	./compile.py corpora/$(CORPUS_NAME)/src_clean.txt corpora/$(CORPUS_NAME)/trg_clean.txt corpora/$(CORPUS_NAME)/corpus_lines.txt corpora/$(CORPUS_NAME)/src_filtered.txt corpora/$(CORPUS_NAME)/trg_filtered.txt

corpora/$(CORPUS_NAME)/corpus_lines.txt: corpora/$(CORPUS_NAME)/corpus.indexed dictionaries/$(DICTIONARY_NAME).lem
	./get_corpus_lines.py dictionaries/$(DICTIONARY_NAME).lem corpora/$(CORPUS_NAME)/report.txt > $@

corpora/$(CORPUS_NAME)/corpus.indexed: corpora/$(CORPUS_NAME)/corpus.split
	./load_corpus.sh corpora/$(CORPUS_NAME)/csv/
	touch corpora/$(CORPUS_NAME)/corpus.indexed

corpora/$(CORPUS_NAME)/corpus.split: corpora/$(CORPUS_NAME)/src.csv
	mkdir corpora/$(CORPUS_NAME)/csv
	split -l $(CORPUS_CHUNK_SIZE) -d --additional-suffix=".csv" $< corpora/$(CORPUS_NAME)/csv/src
	touch corpora/$(CORPUS_NAME)/corpus.split

corpora/$(CORPUS_NAME)/src.csv: corpora/$(CORPUS_NAME)/src_clean.lem
	./lem2csv.py $< > $@


dictionaries/$(DICTIONARY_NAME).lem: dictionaries/$(DICTIONARY_NAME).txt
	mono ../LemmaGenSentenceLemmatizer/LemmaGenSentenceLemmatizer/bin/Debug/LemmaGenSentenceLemmatizer.exe $(SRC_LANG) < $< | sort -u > $@

corpora/$(CORPUS_NAME)/trg_clean.lem: corpora/$(CORPUS_NAME)/trg_clean.tok
	mono ../LemmaGenSentenceLemmatizer/LemmaGenSentenceLemmatizer/bin/Debug/LemmaGenSentenceLemmatizer.exe $(TRG_LANG) < $< > $@

corpora/$(CORPUS_NAME)/src_clean.lem: corpora/$(CORPUS_NAME)/src_clean.tok
	mono ../LemmaGenSentenceLemmatizer/LemmaGenSentenceLemmatizer/bin/Debug/LemmaGenSentenceLemmatizer.exe $(SRC_LANG) < $< > $@

corpora/$(CORPUS_NAME)/src_clean.txt corpora/$(CORPUS_NAME)/trg_clean.txt corpora/$(CORPUS_NAME)/src_clean.tok corpora/$(CORPUS_NAME)/trg_clean.tok: corpora/$(CORPUS_NAME)/src.txt corpora/$(CORPUS_NAME)/trg.txt corpora/$(CORPUS_NAME)/src.tok corpora/$(CORPUS_NAME)/trg.tok
	../clean_corpus.py corpora/$(CORPUS_NAME)/src.txt corpora/$(CORPUS_NAME)/trg.txt corpora/$(CORPUS_NAME)/src.tok corpora/$(CORPUS_NAME)/trg.tok corpora/$(CORPUS_NAME)/src_clean.txt corpora/$(CORPUS_NAME)/trg_clean.txt corpora/$(CORPUS_NAME)/src_clean.tok corpora/$(CORPUS_NAME)/trg_clean.tok $(SEPARATOR)

corpora/$(CORPUS_NAME)/trg.tok: corpora/$(CORPUS_NAME)/trg.txt
	/usr/local/bin/concordia-sentence-tokenizer -c ../../concordia.cfg < $< > $@

corpora/$(CORPUS_NAME)/src.tok: corpora/$(CORPUS_NAME)/src.txt
	/usr/local/bin/concordia-sentence-tokenizer -c ../../concordia.cfg < $< > $@

clean-filtering:
	rm -f corpora/$(CORPUS_NAME)/src_filtered.txt
	rm -f corpora/$(CORPUS_NAME)/trg_filtered.txt
	rm -f corpora/$(CORPUS_NAME)/corpus_lines.txt
	rm -f corpora/$(CORPUS_NAME)/report.txt
	rm -f dictionaries/$(DICTIONARY_NAME).lem


clean:
	rm -f corpora/$(CORPUS_NAME)/src_filtered.txt
	rm -f corpora/$(CORPUS_NAME)/trg_filtered.txt
	rm -f corpora/$(CORPUS_NAME)/corpus.indexed
	rm -f corpora/$(CORPUS_NAME)/corpus.split
	rm -f corpora/$(CORPUS_NAME)/report.txt
	./clear_solr_index.sh
	rm -rf corpora/$(CORPUS_NAME)/csv
	rm -f corpora/$(CORPUS_NAME)/src.csv
	rm -f corpora/$(CORPUS_NAME)/corpus_lines.txt
	rm -f dictionaries/$(DICTIONARY_NAME).lem
	rm -f corpora/$(CORPUS_NAME)/*.lem
	rm -f corpora/$(CORPUS_NAME)/*.tok
	rm -f corpora/$(CORPUS_NAME)/src_clean.txt
	rm -f corpora/$(CORPUS_NAME)/trg_clean.txt
