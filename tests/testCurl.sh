#!/bin/sh

curl -H "Content-Type: application/json" -X POST -d '{"operation":"addSentence", "sourceSentence":"zupełnie nowe zdanie", "targetSentence":"zażółć gęślą jaźńZAŻÓŁĆ GĘŚLĄ JAŹŃ", "tmId":1234782314}' http://localhost


#curl -H "Content-Type: application/json" -X POST -d '{"operation":"simpleSearch", "sentence":"zupełnie nowe"}' http://localhost

