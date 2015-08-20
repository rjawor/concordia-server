#!/bin/sh

# add sentence
#curl -H "Content-Type: application/json" -X POST -d '{"operation":"addSentence", "sourceSentence":"I jeszcze jedno zdanie testowe", "targetSentence":"Yet another test sentence", "tmId":1}' http://localhost

# add sentences
#curl -H "Content-Type: application/json" -X POST -d '{"operation":"addSentences", "sentences":[[1,"test source one", "test target one"],[4,"test source two", "test target two"],[9,"test source three", "test target three"],[13,"test source four", "test target four"]]}' http://localhost

# simple search
curl -H "Content-Type: application/json" -X POST -d '{"operation":"simpleSearch", "pattern":"test source"}' http://localhost


