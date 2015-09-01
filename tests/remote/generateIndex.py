#!/usr/bin/python
# -*- coding: utf-8 -*-

import unittest
import json
import urllib2
import sys
import time

print "Generating index..."
start = time.time()
data = {
    'operation': 'refreshIndex'
}
req = urllib2.Request('http://concordia.vm.wmi.amu.edu.pl:8800/')
req.add_header('Content-Type', 'application/json')
urllib2.urlopen(req, json.dumps(data)).read()

end = time.time()
print "Index regeneration complete. The operation took %.4f s" % (end - start)



