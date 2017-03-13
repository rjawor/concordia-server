#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys, os, shutil, re

root_dir = sys.argv[1]

if not os.path.exists(root_dir):
    print "%s does not exist!" % root_dir
    sys.exit(1)

if not os.path.isdir(root_dir):
    print "%s is not a directory!" % root_dir
    sys.exit(1)

if len(os.listdir(root_dir))>0:
    print "%s is not empty!" % root_dir
    sys.exit(1)

shutil.copytree('js', root_dir+'/js')
shutil.copytree('css', root_dir+'/css')
shutil.copytree('images', root_dir+'/images')
shutil.copy('favicon.ico', root_dir+'/favicon.ico')


with open('host.cfg', 'r') as host_file:
    for line in host_file:
        field, value = line.strip().split('@#@')
        if field == 'concordia_host':
            concordia_host = value
        elif field == 'concordia_port':
            concordia_port = value

with open('concordia_gate.php_pattern', 'r') as gate_pattern_file, open(root_dir+'/concordia_gate.php', 'w') as gate_file:
    for line in gate_pattern_file:
        line = re.sub('@concordia_host@', concordia_host, line)
        line = re.sub('@concordia_port@', concordia_port, line)
        gate_file.write(line)

with open('concordia_search.php_pattern', 'r') as search_pattern_file, open(root_dir+'/concordia_search.php', 'w') as search_file:
    for line in search_pattern_file:
        line = re.sub('@concordia_host@', concordia_host, line)
        line = re.sub('@concordia_port@', concordia_port, line)
        search_file.write(line)

with open('tm_info.php_pattern', 'r') as tm_info_pattern_file, open(root_dir+'/tm_info.php', 'w') as tm_info_file:
    for line in tm_info_pattern_file:
        line = re.sub('@concordia_host@', concordia_host, line)
        line = re.sub('@concordia_port@', concordia_port, line)
        tm_info_file.write(line)


versions_dir = 'versions_enabled'

versions = []

for version_file in os.listdir(versions_dir):
    version = {'suggestions':[]}
    with open(versions_dir+'/'+version_file) as v:
        for line in v:

            field, value = line.strip().split('@#@')
            if field == 'suggestion':
                version['suggestions'].append(value)
            else:
                version[field] = value
    versions.append(version)






for version in versions:
    version_dir = root_dir+'/'+version['dir']
    os.mkdir(version_dir)
    with open('index.html_pattern', 'r') as pattern_file:
        with open(version_dir+'/index.html', 'w') as index_file:
            for line in pattern_file:
                for field, value in version.iteritems():
                    if field == 'suggestions':
                        suggestions_html = ''
                        for suggestion in value:
                            suggestions_html+='<li>'+suggestion+' <span class="suggestion" onclick="searchText(\''+suggestion+'\', '+version['tmid']+');">apply</span></li>'
                        line = re.sub('@suggestions@', suggestions_html, line)
                    else:
                        line = re.sub('@'+field+'@', value, line)
                index_file.write(line)
