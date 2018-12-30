#flask stuff
from flask import Flask
from flask import render_template
from flask import request
from flask import send_from_directory
#regexes
import re
#scripting stuff
import subprocess
import os
import signal

from subprocess import Popen, PIPE, TimeoutExpired
from time import monotonic as timer

app = Flask(__name__)
TIMEOUT=30

@app.route("/")
def nocommand():
    return send_from_directory('static', 'index.html')


@app.route('/prove', methods=['POST', 'GET'])
def prove():
    error = None

    premises = request.form['premises']
    #print('DEBUG: request.form[premises]'+premises)
    conclusion = request.form['conclusion']
    #print('DEBUG: request.form[conclusion]'+conclusion)

    premises = ''.join(premises.split()) #strip all whitespace
    conclusion = ''.join(conclusion.split()) #strip all whitespace

    if request.method == 'POST':
        if validate_argument(premises, conclusion):
            return prove_argument(premises, conclusion)
        else:
            error = 'Could not validate argument'
    elif request.method == 'GET':
        error = 'GET requests are not yet implemented'
    else:
        error = 'Invalid request'
    return render_template('error.html', error=error)


# check input argument for bad characters and etc
def validate_argument(premises, conclusion):
    #legal_chars = r'^[a-zA-Z->()& ;~]*$' #for now disable lowercase chars
    legal_chars = r'^[A-Zv\->()& ;~]*$' #The upper case alphabetic chars,v,-,>,(,),&,space,;,~
    ban_chars = r'[\'@#$%^*\[\]:\"<?,.|`]'
    ban_words = r'eval|sh|cd|pwd|python'
    bad_syntax = r'\([A-Z]\)|~\([A-Z]\)'
    retval = True
    if re.search(bad_syntax, premises):
        print("CASE 3")
        retval = False
    if re.search(bad_syntax, conclusion):
        print("CASE 4")
        retval = False
    if re.search(ban_words, premises):
        print("CASE 3")
        retval = False
    if re.search(ban_words, conclusion):
        print("CASE 4")
        retval = False
    if re.search(ban_chars, premises):
        print("CASE 5")
        retval = False
    if re.search(ban_chars, conclusion):
        print("CASE 6")
        retval = False
    if not re.search(legal_chars, premises):
        print("CASE 7")
        retval = False
    if not re.search(legal_chars, conclusion):
        print("CASE 8")
        retval = False
    return retval

def prove_argument(premises, conclusion):
    print("-----------------------------------------------------------------------")
    print("DEBUG: prove_argument() new proof")
    linear_proof = proof_exprtest(premises, conclusion)
    #print("DEBUG linear_proof:"+linear_proof)
    graph_proof = proof_exprgraph(premises, conclusion)
    return render_template('proof.html', premises=premises.replace(";","<br>"), conclusion=conclusion, linear_proof=linear_proof,
                           graph_proof=graph_proof)


def proof_exprgraph(premises, conclusion):
    starting_dir = os.getcwd()
    build_translate_file(premises, conclusion,
                         "/home/mehstruslehpy3/exprweb/exprGraph/proofTranslator/transfile_heredoc.txt")
    os.chdir("/home/mehstruslehpy3/exprweb/exprGraph/proofTranslator")

    subprocess.call(["chmod", "+x", "transfile_heredoc.txt"])
    #subprocess.call("./transfile_heredoc.txt", shell=True)
    print("DEBUG: running /home/mehstruslehpy3/exprweb/exprGraph/proofTranslator/transfile_heredoc.txt")
    run("/home/mehstruslehpy3/exprweb/exprGraph/proofTranslator/transfile_heredoc.txt",TIMEOUT)
    subprocess.call(["chmod", "+x", "output_heredoc.txt"])
    #subprocess.call("./output_heredoc.txt", shell=True)
    print("DEBUG: running /home/mehstruslehpy3/exprweb/exprGraph/proofTranslator/output_heredoc.txt")
    run("/home/mehstruslehpy3/exprweb/exprGraph/proofTranslator/output_heredoc.txt",TIMEOUT)
    print("DEBUG: running /home/mehstruslehpy3/exprweb/exprGraph/proofTranslator/buildsvg.sh")
    run("/home/mehstruslehpy3/exprweb/exprGraph/proofTranslator/buildsvg.sh",TIMEOUT)
    #subprocess.call("./buildsvg.sh")

    #outfile = open("proveroutput.txt", 'r')
    outfile = open("proveroutput.svg", 'r')

    proof = outfile.readlines()
    outfile.close()
    outstr = ""
    i = 0
    while (i < len(proof)):
        outstr += proof[i]
        i += 1
    subprocess.call(["rm", "proveroutput.txt"])
    subprocess.call(["rm", "output_heredoc.txt"])
    subprocess.call(["rm", "transfile_heredoc.txt"])
    subprocess.call(["rm", "proveroutput.svg"])
    subprocess.call(["rm", "OUTPUT_DOTFILE_PROOF.gv"])
    #print("DEBUG: ls-altr:")
    #subprocess.call(["ls","-altr"])
    os.chdir(starting_dir)
    return outstr


def proof_exprtest(premises, conclusion):
    print("DEBUG: proof_exprtest()")
    starting_dir = os.getcwd()
    build_translate_file(premises, conclusion,
                         "/home/mehstruslehpy3/exprweb/exprTest/proofTranslator/transfile_heredoc.txt")
    os.chdir("/home/mehstruslehpy3/exprweb/exprTest/proofTranslator")
    subprocess.call(["chmod", "+x", "transfile_heredoc.txt"])
    #subprocess.call("./transfile_heredoc.txt", shell=True)
    print("DEBUG: running /home/mehstruslehpy3/exprweb/exprTest/proofTranslator/transfile_heredoc.txt")
    run("/home/mehstruslehpy3/exprweb/exprTest/proofTranslator/transfile_heredoc.txt",TIMEOUT)
    subprocess.call(["chmod", "+x", "output_heredoc.txt"])
    print("DEBUG: running /home/mehstruslehpy3/exprweb/exprTest/proofTranslator/output_heredoc.txt")
    run("/home/mehstruslehpy3/exprweb/exprTest/proofTranslator/output_heredoc.txt",TIMEOUT)
    #subprocess.call("./output_heredoc.txt", shell=True)
    #subprocess.call(["rm", "output_heredoc.txt", "transfile_heredoc.txt"])

    outfile = open("proveroutput.txt", 'r')

    proof = outfile.readlines()
    outfile.close()

    i = 0
    outstr = ""
    while (i < len(proof)):
        proof[i] = proof[i].replace('\n', ' <br> ')
        outstr += proof[i]
        i += 1
    subprocess.call(["rm", "proveroutput.txt"])
    subprocess.call(["rm", "output_heredoc.txt"])
    subprocess.call(["rm", "transfile_heredoc.txt"])
    subprocess.call(["rm", "proveroutput.txt"])
    #print("DEBUG: ls-altr:")
    #subprocess.call(["ls","-altr"])
    os.chdir(starting_dir)
    return outstr


def build_translate_file(premises, conclusion, path):
    #subprocess.call(["ls","-altr"])
    outfile = open(path, 'w')
    outfile.write("#!/bin/sh\n")
    outfile.write("./translator << ENDOFMESSAGE\n")
    #premises_list = premises.split(";", len(premises_list))
    premises_list = premises.split(";")

    for str in premises_list:
        outfile.write(""+str+"\n")
        outfile.write("p\n")
    outfile.close()

    outfile = open(path, 'r')
    lines = outfile.readlines()
    outfile.close()
    outfile = open(path, 'w')
    outfile.writelines([item for item in lines[:-1]])

    outfile.write("c\n")
    outfile.write(conclusion+"\n")
    outfile.write("ENDOFMESSAGE\n")
    outfile.close()
    return "SAMPLE OUTPUT"

#from SO runs command and terminates after a timeout period if process does not end
def run(cmd, timeout_sec):
    start = timer()
    with Popen(cmd, shell=True, stdout=PIPE, preexec_fn=os.setsid) as process:
        try:
            output = process.communicate(timeout=timeout_sec)[0]
        except TimeoutExpired:
            os.killpg(process.pid, signal.SIGINT) # send signal to the process group
            output = process.communicate()[0]
            print("DEBUG: process timed out: "+cmd)
    print('DEBUG: Elapsed seconds: {:.2f}'.format(timer() - start))
    #proc = Popen(shlex.split(cmd), stdout=PIPE, stderr=PIPE,shell=False)
    #timer = Timer(timeout_sec, proc.kill)
    #print("DEBUG: before try")
    #try:
        #print("DEBUG: within try")
        #timer.start()
        #stdout, stderr = proc.communicate()
    #except:
    #    print("DEBUG: process ran too long")
    #finally:
        #print("DEBUG: within finally")
        #timer.cancel()
