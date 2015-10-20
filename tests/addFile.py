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
    urllib2.urlopen(req, json.dumps(data)).read()

sourceFile = sys.argv[1]
sourceLangId = int(sys.argv[2])
targetFile = sys.argv[3]
targetLangId = int(sys.argv[4])
name = sys.argv[5]

totalLines = file_len(sourceFile)
if file_len(targetFile) != totalLines:
    print "File lengths do not match"
    sys.exit(1)
    
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
    'operation': 'addSentences',
    'tmId':tmId
}

sentences = []
start = time.time()
with open(sourceFile) as sourceSentences:
    with open(targetFile) as targetSentences:
        lineNumber = 0
        for sourceSentence in sourceSentences:
            lineNumber += 1
            targetSentence = targetSentences.readline()
            sentences.append([sourceSentence, targetSentence])
            if lineNumber % BUFFER_SIZE == 0:
                data['sentences'] = sentences
                sentences = []
                add_data(data)
                mark = time.time()
                print "Added %d of %d sentences. Time elapsed: %.4f s, current speed: %.4f sentences/second" % (lineNumber, totalLines, mark-start, lineNumber/(mark-start))
                

if len(sentences) > 0:
    data['sentences'] = sentences
    add_data(data)
    
end = time.time()
print "Added all %d sentences. Time elapsed: %.4f s, overall speed: %.4f sentences/second" % (lineNumber, end-start, lineNumber/(end-start))

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



