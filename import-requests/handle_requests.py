#!/usr/bin/python3
# -*- coding: utf-8 -*-

import psycopg2, os, shutil, subprocess, urllib3, time, json

from importlib.machinery import SourceFileLoader

BUFFER_SIZE = 500

def postJson(address, data):
    http = urllib3.PoolManager()
    response = http.request('POST', address, headers={'Content-Type': 'application/json'},body=json.dumps(data).encode('utf-8'))

    return json.loads(response.data.decode('utf-8'))


def add_examples(address, examplesData):
    response = postJson(address, examplesData)
    if response['status'] == 'error':
        raise Exception(response['message'])

def file_len(fname):
    with open(fname) as f:
        for i, l in enumerate(f):
            pass
    return i + 1


mgiza_path = os.path.dirname(os.path.realpath(__file__))+'/../mgiza-aligner'

conn = psycopg2.connect("dbname='concordia_server' user='concordia' host='localhost' port='6543' password='concordia'")
cur = conn.cursor()
cur.execute("""select request.id, request.source_file_path, request.target_file_path, request.name, src_lang.id as src_lang_id, src_lang.code as src_code, trg_lang.id as trg_lang_id, trg_lang.code as trg_code, request.status, request.type, request.tm_id from request inner join language as src_lang on src_lang.id = request.source_lang_id inner join language as trg_lang on trg_lang.id = request.target_lang_id order by request.created limit 1""")


request = cur.fetchone()
request_id, src_file_path, trg_file_path, tm_name, src_lang_id, src_lang_code, trg_lang_id, trg_lang_code, status, tm_type, tm_id  = request
request_corpus_path = mgiza_path+'/corpora/request_'+str(request_id)
os.makedirs(request_corpus_path)
shutil.copy(src_file_path, request_corpus_path+'/src.txt')
shutil.copy(trg_file_path, request_corpus_path+'/trg.txt')

subprocess.run(["make","SRC_LANG="+src_lang_code, "TRG_LANG="+trg_lang_code, "CORPUS_NAME=request_"+str(request_id)], cwd=mgiza_path)

cur.close()
conn.close()

host = SourceFileLoader("host", os.path.dirname(os.path.realpath(__file__))+'/../tests/host.py').load_module()
address = 'http://'+host.concordia_host
if len(host.concordia_port) > 0:
    address += ':'+host.concordia_port


sourceFile = request_corpus_path+'/src_final.txt'
targetFile = request_corpus_path+'/trg_final.txt'
alignmentsFile = request_corpus_path+'/aligned_final.txt'

if (file_len(sourceFile) != file_len(targetFile)):
    raise Exception("source and target files are not of the same length!")

if (file_len(alignmentsFile) != 3*file_len(sourceFile)):
    raise Exception("alignments file is not exactly 3 times longer than source and target")

data = {
    'operation': 'addTm',
    'sourceLangId':src_lang_id,
    'targetLangId':trg_lang_id,
    'name':tm_name,
    'tmLemmatized':True
}

response = postJson(address, data)

tmId = int(response['newTmId'])
print("Added new tm: %d" % tmId)

data = {
    'operation': 'addAlignedLemmatizedSentences',
    'tmId':tmId
}

examples = []
with open(sourceFile) as sf, open(targetFile) as tf, open(alignmentsFile) as af:
    for sourceLine in sf:
        sourceSentence = sourceLine.strip()
        targetSentence = tf.readline().strip()

        # skip to lines of the alignments file, these are lemmatized and we need the raw sentences from the source and target files.
        af.readline()
        af.readline()

        alignmentString = af.readline().strip()

        examples.append([sourceSentence, targetSentence, alignmentString])

        if len(examples) >= BUFFER_SIZE:
            data['examples'] = examples
            add_examples(address, data)
            examples = []

if len(examples) > 0:
    data['examples'] = examples
    add_examples(address, data)

print("Generating index...")
start = time.time()
data = {
    'operation': 'refreshIndex',
    'tmId' : tmId
}

response = postJson(address, data)

end = time.time()
print("Index regeneration complete. The operation took %.4f s" % (end - start))
