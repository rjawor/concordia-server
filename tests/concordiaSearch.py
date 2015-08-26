#!/usr/bin/python
# -*- coding: utf-8 -*-

import unittest
import json
import urllib2
import sys
import time

data = {
    'operation': 'concordiaSearch',
    'pattern':sys.argv[1]
}

start = time.time()
req = urllib2.Request('http://localhost')
req.add_header('Content-Type', 'application/json')
response = json.loads(urllib2.urlopen(req, json.dumps(data)).read())
end = time.time()

print "Execution time: %.4f seconds." % (end-start)
print "Result: "
print response

