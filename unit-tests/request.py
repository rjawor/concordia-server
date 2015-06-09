#!/usr/bin/python
# -*- coding: utf-8 -*-

import json
import urllib2

data = {
        'operation': 'python',
        'data':'foo'
}

req = urllib2.Request('http://localhost')
req.add_header('Content-Type', 'application/json')

response = urllib2.urlopen(req, json.dumps(data)).read()

print response
