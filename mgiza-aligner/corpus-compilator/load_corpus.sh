#!/bin/sh

CORPUS_FOLDER=$1

CHUNKS_COUNT=`ls $CORPUS_FOLDER | wc -l`

echo "Total file count" $CHUNKS_COUNT
INDEX=0
for FILE in `ls $CORPUS_FOLDER`
do
    INDEX=`expr $INDEX + 1`
    echo "Working on file" $INDEX "of" $CHUNKS_COUNT
    curl -X POST --data-binary @$CORPUS_FOLDER$FILE -H 'Content-type:application/csv' \
      'http://localhost:8983/solr/corpus_compiler/update?commit=true&optimize=true&separator=%09&fieldnames=line_number,content'
done
