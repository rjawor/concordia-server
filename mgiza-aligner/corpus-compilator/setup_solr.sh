#!/bin/sh

SOLR_HOME=/home/rafalj/programs/solr-6.0.0

$SOLR_HOME/bin/solr restart
$SOLR_HOME/bin/solr create -c corpus_compiler


curl -X POST -H 'Content-type:application/json'  http://localhost:8983/solr/corpus_compiler/schema -d '{
  "add-field":{ "name":"content", "type":"text_general"},
  "add-field":{ "name":"line_number", "type":"int"}
}'
