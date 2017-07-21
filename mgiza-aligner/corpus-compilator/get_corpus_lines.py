#!/usr/bin/python3
# -*- coding: utf-8 -*-

import sys, json, requests

dictionary_path = sys.argv[1]
report_path = sys.argv[2]


dictionary_frequencies = []
filtered_corpus_lines = set([])

with open(dictionary_path) as dictionary:
    index = 0
    for line in dictionary:
        index += 1
        word = line.rstrip()
        request_url = 'http://localhost:8983/solr/corpus_compiler/select?q='
        request_url += '"'+word+'"'
        request_url +='&rows=100000&wt=json'
        response = requests.get(request_url)
        try:
            json_response = json.loads(response.content.decode('utf-8'))
            if json_response['response']['numFound'] > 0:
                dictionary_frequencies.append((word,json_response['response']['numFound']))
                for doc in json_response['response']['docs']:
                    filtered_corpus_lines.add(doc['line_number'])
        except:
            pass
        if index % 100 == 0:
            sys.stderr.write("Done %d dictionary words. Current corpus size: %d\n" % (index, len(filtered_corpus_lines)))


for number in sorted(filtered_corpus_lines):
    print(str(number))

with open(report_path, 'w') as report:
    for entry in sorted(dictionary_frequencies, key=lambda x:-x[1]):
        report.write("%s\t%d\n" % entry)
