#!/usr/bin/python
# -*- coding: utf-8 -*-

import unittest
from simpleSearchTest import SimpleSearchTest
from concordiaSearchTest import ConcordiaSearchTest

simpleSearchSuite = unittest.TestLoader().loadTestsFromTestCase(SimpleSearchTest)
concordiaSearchSuite = unittest.TestLoader().loadTestsFromTestCase(ConcordiaSearchTest)
alltests = unittest.TestSuite([simpleSearchSuite, concordiaSearchSuite])
unittest.TextTestRunner(verbosity=2).run(alltests)
