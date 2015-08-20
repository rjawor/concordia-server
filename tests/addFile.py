#!/usr/bin/python
# -*- coding: utf-8 -*-

import unittest
import json
import urllib2
import sys
import time

def file_len(fname):
    with open(fname) as f:
        for i, l in enumerate(f):
            pass
    return i + 1

def add_data(data):
    req = urllib2.Request('http://localhost')
    req.add_header('Content-Type', 'application/json')
    urllib2.urlopen(req, json.dumps(data)).read()

sourceFile = sys.argv[1]
targetFile = sys.argv[2]
tmId = int(sys.argv[3])

totalLines = file_len(sourceFile)
if file_len(targetFile) != totalLines:
    print "File lengths do not match"
    sys.exit(1)

BUFFER_SIZE = 1000

data = {
    'operation': 'addSentences'
}

sentences = []
start = time.time()
with open(sys.argv[1]) as sourceSentences:
    with open(sys.argv[2]) as targetSentences:
        lineNumber = 0
        for sourceSentence in sourceSentences:
            lineNumber += 1
            targetSentence = targetSentences.readline()
            sentences.append([tmId, sourceSentence, targetSentence])
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
    'operation': 'refreshIndex'
}
req = urllib2.Request('http://localhost')
req.add_header('Content-Type', 'application/json')
urllib2.urlopen(req, json.dumps(data)).read()

end = time.time()
print "Index regeneration complete. The operation took %.4f s" % (end - start)



