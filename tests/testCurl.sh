#!/bin/sh

#curl -H "Content-Type: application/json" -X POST -d '{"operation":"addSentence", "sourceSentence":"Marysia ma rysia", "targetSentence":"Mary has a bobcat", "tmId":1}' http://localhost


curl -H "Content-Type: application/json" -X POST -d '{"operation":"simpleSearch", "pattern":"ma rysia"}' http://localhost

