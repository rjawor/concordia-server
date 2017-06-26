#!/usr/bin/python3
# -*- coding: utf-8 -*-

import psycopg2, os, sys, shutil, subprocess, urllib3, time, json, logging, errno

from importlib.machinery import SourceFileLoader

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


def pid_exists(pid):
    """Check whether pid exists in the current process table.
    UNIX only.
    """
    if pid < 0:
        return False
    if pid == 0:
        # According to "man 2 kill" PID 0 refers to every process
        # in the process group of the calling process.
        # On certain systems 0 is a valid PID but we have no way
        # to know that in a portable fashion.
        raise ValueError('invalid PID 0')
    try:
        os.kill(pid, 0)
    except OSError as err:
        if err.errno == errno.ESRCH:
            # ESRCH == No such process
            return False
        elif err.errno == errno.EPERM:
            # EPERM clearly means there's a process to deny access to
            return True
        else:
            # According to "man 2 kill" possible error values are
            # (EINVAL, EPERM, ESRCH)
            raise
    else:
        return True

def updateRequestStatus(connection_string, request_id, new_status):
    conn = psycopg2.connect(connection_string)
    cur = conn.cursor()
    cur.execute("UPDATE request SET status=%s WHERE id=%s", (new_status, request_id))
    conn.commit()
    cur.close()
    conn.close()

def getPendingRequest(connection_string):
    conn = psycopg2.connect(connection_string)
    cur = conn.cursor()
    cur.execute("""select request.id, request.source_file_path, request.target_file_path, request.name, src_lang.id as src_lang_id, src_lang.code as src_code, trg_lang.id as trg_lang_id, trg_lang.code as trg_code, request.status, request.type, request.tm_id from request inner join language as src_lang on src_lang.id = request.source_lang_id inner join language as trg_lang on trg_lang.id = request.target_lang_id where request.status = 0 order by request.created limit 1""")

    request = cur.fetchone()
    cur.close()
    conn.close()
    return request

def retrieve_and_append(connection_string, tm_id, src_file_path, trg_file_path):
    conn = psycopg2.connect(connection_string)
    cur = conn.cursor()
    cur.execute("SELECT source_segment, target_segment FROM unit WHERE tm_id=%d" % tm_id)
    with open(src_file_path, 'a') as src_file, open(trg_file_path, 'a') as trg_file:
        for row in cur:
            src_file.write(row[0]+'\n')
            trg_file.write(row[1]+'\n')

    cur.close()
    conn.close()

def clearTm(connection_string, tm_id):
    conn = psycopg2.connect(connection_string)
    cur = conn.cursor()
    cur.execute("DELETE FROM unit WHERE tm_id=%d" %tm_id)
    conn.commit()
    cur.close()
    conn.close()

def getLangsFromTM(connection_string, tm_id):
    conn = psycopg2.connect(connection_string)
    cur = conn.cursor()
    cur.execute("select src_lang.code, trg_lang.code from tm inner join language as src_lang on tm.source_lang_id = src_lang.id inner join language as trg_lang on tm.target_lang_id = trg_lang.id where tm.id = %d" % tm_id)
    langs = cur.fetchone()
    cur.close()
    conn.close()
    return langs

# ----------- main -------------------------------------------------------------

BUFFER_SIZE = 500

src_dir = os.path.dirname(os.path.realpath(__file__))

logger = logging.getLogger('request_handler')
hdlr = logging.FileHandler(src_dir+'/request_handler.log')
formatter = logging.Formatter('%(asctime)s %(levelname)s %(message)s')
hdlr.setFormatter(formatter)
logger.addHandler(hdlr)
logger.setLevel(logging.INFO)


pidfile_path = src_dir+'/request_handler.pid'
if os.path.isfile(pidfile_path):
    with open(pidfile_path) as pidfile:
        pid = int(pidfile.readline().strip())
        logger.info('Found pid file with pid: %d' % pid)
        if pid_exists(pid):
            logger.info('The process with pid=%d is actually running, exiting' % pid)
            sys.exit(0)
        else:
            logger.warning('The process with pid=%d is not running anymore. Ignoring the pid file.' % pid)

with open(pidfile_path, 'w') as pidfile:
    pidfile.write(str(os.getpid()))

mgiza_path = src_dir+'/../mgiza-aligner'

connection_string = "dbname='concordia_server' user='concordia' host='localhost' port='6543' password='concordia'"

try:
    request = getPendingRequest(connection_string)
    if request is not None:
        request_id, src_file_path, trg_file_path, tm_name, src_lang_id, src_lang_code, trg_lang_id, trg_lang_code, status, tm_type, tm_id  = request

        if tm_type == 1:
            src_lang_code, trg_lang_code = getLangsFromTM(connection_string, tm_id)

        logger.info("Working on request: %d. Starting the mgiza aligner." % request_id)
        updateRequestStatus(connection_string, request_id, 1)

        request_corpus_path = mgiza_path+'/corpora/request_'+str(request_id)
        os.makedirs(request_corpus_path)

        shutil.copy(src_file_path, request_corpus_path+'/src.txt')
        shutil.copy(trg_file_path, request_corpus_path+'/trg.txt')

        if tm_type == 1:
            # extending existing TM. We must retrieve existing units from the db.
            logger.info("This is an extend request. Appending units from the tm %d to the src and trg files."%tm_id)
            retrieve_and_append(connection_string, tm_id, request_corpus_path+'/src.txt', request_corpus_path+'/trg.txt')

        subprocess.run(["make","SRC_LANG="+src_lang_code, "TRG_LANG="+trg_lang_code, "CORPUS_NAME=request_"+str(request_id)], cwd=mgiza_path, check=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)

        logger.info("Mgiza alignment complete, starting adding the TM to Concordia.")
        updateRequestStatus(connection_string, request_id, 2)

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

        if tm_type == 0:
            # create new TM
            data = {
                'operation': 'addTm',
                'sourceLangId':src_lang_id,
                'targetLangId':trg_lang_id,
                'name':tm_name,
                'tmLemmatized':True
            }

            response = postJson(address, data)

            tmId = int(response['newTmId'])
            logger.info("Added new tm: %d" % tmId)
        else:
            clearTm(connection_string, tm_id)
            tmId = tm_id

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

        updateRequestStatus(connection_string, request_id, 3)
        logger.info("Generating index...")
        start = time.time()
        data = {
            'operation': 'refreshIndex',
            'tmId' : tmId
        }

        response = postJson(address, data)

        end = time.time()
        logger.info("Index regeneration complete. The operation took %.4f s" % (end - start))

except Exception as e:
    logger.error("Error occured: "+str(e))
    updateRequestStatus(connection_string, request_id, 5)
else:
    updateRequestStatus(connection_string, request_id, 4)
finally:
    os.remove(pidfile_path)
