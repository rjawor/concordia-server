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

def add_examples(examplesData):
    req = urllib2.Request(address)
    req.add_header('Content-Type', 'application/json')
    response = json.loads(urllib2.urlopen(req, json.dumps(examplesData)).read())
    if response['status'] == 'error':
        raise Exception(response['message'])

if len(sys.argv) != 7:
    raise Exception("wrong number of arguments")

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


totalExamples = file_len(sourceFile)

data = {
    'operation': 'addTm',
    'sourceLangId':sourceLangId,
    'targetLangId':targetLangId,
    'name':name,
    'tmLemmatized':True
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

examples = []
start = time.time()
with open(sourceFile) as sf, open(targetFile) as tf, open(alignmentsFile) as af:
    for lineNumber in range(totalExamples):
        sourceSentence = sf.readline().strip()
        targetSentence = tf.readline().strip()

        # skip to lines of the alignments file, these are lemmatized and we need the raw sentences from the source and target files.
        af.readline()
        af.readline()

        alignmentString = af.readline().strip()

        examples.append([sourceSentence, targetSentence, alignmentString])

        if len(examples) >= BUFFER_SIZE:
            data['examples'] = examples
            add_examples(data)
            mark = time.time()
            print "Added %d of %d lemmatized examples. Time elapsed: %.4f s, current speed: %.4f examples/second" % ( (lineNumber+1), totalExamples, mark-start, (lineNumber+1)/(mark-start))
            examples = []


if len(examples) > 0:
    data['examples'] = examples
    add_examples(data)

end = time.time()
print "Added all %d lemmatized sentences. Time elapsed: %.4f s, overall speed: %.4f sentences/second" % ((lineNumber+1), end-start, (lineNumber+1)/(end-start))

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
