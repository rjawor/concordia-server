#!/usr/bin/python
# -*- coding: utf-8 -*-

import json
import urllib2
import sys
import time
import host

address = 'http://'+host.concordia_host
if len(host.concordia_port) > 0:
    address += ':'+host.concordia_port


data = {
    'operation': 'addTm',
    'sourceLangId':int(sys.argv[1]),
    'targetLangId':int(sys.argv[2]),
    'name':sys.argv[3]
}

req = urllib2.Request(address)
req.add_header('Content-Type', 'application/json')
response = json.loads(urllib2.urlopen(req, json.dumps(data)).read())

print response
