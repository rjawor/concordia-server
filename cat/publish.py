#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys, os, shutil, re

def config_file(config, file_name, root_dir):
    with open(file_name+'_pattern', 'r') as pattern_file, open(root_dir+'/'+file_name, 'w') as out_file:
        for line in pattern_file:
            for field, value in config.iteritems():
                line = re.sub('@'+field+'@', value, line)
            out_file.write(line)

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


config = dict()
with open('host.cfg', 'r') as host_file:
    for line in host_file:
        field, value = line.strip().split('@#@')
        config[field] = value

config_file(config, 'concordia_gate.php', root_dir)
config_file(config, 'concordia_search.php', root_dir)
config_file(config, 'tm_info.php', root_dir)
config_file(config, 'tm_manager.php', root_dir)


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
