#!/bin/sh

#curl -H "Content-Type: application/json" -X POST -d '{"operation":"addSentence", "sentence":"zupełnie nowe zdanie"}' http://localhost
curl -H "Content-Type: application/json" -X POST -d '{"operation":"simpleSearch", "sentence":"zupełnie snowe"}' http://localhost

