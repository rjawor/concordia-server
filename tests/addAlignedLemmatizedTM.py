#!/usr/bin/python
# -*- coding: utf-8 -*-

import unittest
import json
import urllib2
import sys
import host
import time

BUFFER_SIZE = 500

address = 'http://'+host.concordia_host
if len(host.concordia_port) > 0:
    address += ':'+host.concordia_port


def file_len(fname):
    with open(fname) as f:
        for i, l in enumerate(f):
            pass
    return i + 1

def add_data(data):
    req = urllib2.Request(address)
    req.add_header('Content-Type', 'application/json')
    json.loads(urllib2.urlopen(req, json.dumps(data)).read())

name = sys.argv[1]
sourceFile = sys.argv[2]
sourceLangId = int(sys.argv[3])
targetFile = sys.argv[4]
targetLangId = int(sys.argv[5])
alignmentsFile = sys.argv[6]

if (file_len(sourceFile) != file_len(targetFile)):
    raise Exception("source and target files are not of the same length!")

if (file_len(alignmentsFile) != 3*file_len(sourceFile)):
    raise Exception("alignments file is not exactly 3 times longer than source and target")


totalLines = file_len(sourceFile)

data = {
    'operation': 'addTm',
    'sourceLangId':sourceLangId,
    'targetLangId':targetLangId,
    'name':name
}

req = urllib2.Request(address)
req.add_header('Content-Type', 'application/json')
response = json.loads(urllib2.urlopen(req, json.dumps(data)).read())
tmId = int(response['newTmId'])
print "Added new tm: %d" % tmId

data = {
    'operation': 'addAlignedLemmatizedSentences',
    'tmId':tmId
}

sentences = []
start = time.time()
with open(sourceFile) as sourceLines, open(targetFile) as targetLines, open(alignmentsFile) as alignmentsLines:

    lineNumber = 0
    for line in sourceLines:
        line = line.strip()
        if lineNumber % 3 == 1:
            currSentence.append(line)
        elif lineNumber % 3 == 2:
            currSentence.append(line)
            currSentence.reverse()
            sentences.append(currSentence)
            currSentence = []
            if len(sentences) >= BUFFER_SIZE:
                data['sentences'] = sentences
                add_data(data)
                mark = time.time()
                print "Added %d of %d sentences. Time elapsed: %.4f s, current speed: %.4f sentences/second" % ( (lineNumber+1)/3, totalLines/3, mark-start, (lineNumber+1)/(3*(mark-start)))
                sentences = []
        lineNumber += 1


if len(sentences) > 0:
    data['sentences'] = sentences
    add_data(data)

end = time.time()
print "Added all %d sentences. Time elapsed: %.4f s, overall speed: %.4f sentences/second" % ((lineNumber+1)/3, end-start, (lineNumber+1)/(3*(end-start)))

print "Generating index..."
start = time.time()
data = {
    'operation': 'refreshIndex',
    'tmId' : tmId
}
req = urllib2.Request(address)
req.add_header('Content-Type', 'application/json')
urllib2.urlopen(req, json.dumps(data)).read()

end = time.time()
print "Index regeneration complete. The operation took %.4f s" % (end - start)
