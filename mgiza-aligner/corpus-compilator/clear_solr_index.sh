#!/bin/sh

echo "Clearing solr index"

curl "http://localhost:8983/solr/corpus_compiler/update?stream.body=<delete><query>*:*</query></delete>&commit=true"
