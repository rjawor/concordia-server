#!/usr/bin/python
# -*- coding: utf-8 -*-

import unittest
import json
import urllib2
import sys
import time
import host

address = 'http://'+host.concordia_host
if len(host.concordia_port) > 0:
    address += ':'+host.concordia_port


data = {
    'operation': 'concordiaSearch',
    'pattern':sys.argv[1]
    'tmId':int(sys.argv[2])
}

start = time.time()
req = urllib2.Request(address)
req.add_header('Content-Type', 'application/json')
response = json.loads(urllib2.urlopen(req, json.dumps(data)).read())
end = time.time()

print "Execution time: %.4f seconds." % (end-start)
print "Result: "
print response


