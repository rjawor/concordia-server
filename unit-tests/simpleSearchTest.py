#!/usr/bin/python
# -*- coding: utf-8 -*-

import unittest
import json
import urllib2


class SimpleSearchTest(unittest.TestCase):
    def setUp(self):
        print "set up simple search"

    def test_operation(self):
        data = {
            'operation': 'python',
            'data':'foo'
        }
        req = urllib2.Request('http://localhost')
        req.add_header('Content-Type', 'application/json')
        response = json.loads(urllib2.urlopen(req, json.dumps(data)).read())
        
        self.assertEqual(response['status'], 'success')
        self.assertEqual(response['passedOperation'], 'python')

    def test_upper(self):
        self.assertEqual('foo'.upper(), 'FOO')

    def test_isupper(self):
        self.assertTrue('FOO'.isupper())
        self.assertFalse('Foo'.isupper())

    def test_split(self):
        s = 'hello world'
        self.assertEqual(s.split(), ['hello', 'world'])
        # check that s.split fails when the separator is not a string
        with self.assertRaises(TypeError):
            s.split(2)


