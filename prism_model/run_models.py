import os
import subprocess
import csv
import re
import math
from pathlib import Path

currentdir = os.getcwd()
logfilename = "log_results.txt"
pval_filename = "pval_results.txt"

DRN_OFFSET = 0.5
_5HTR2_OFFSET = 1

def ofc5HTreceptors(_5ht,x,t):
    if t == 0:
        return (1-math.exp(-pow(x/(_5ht+1),(_5ht+1))))*(_5ht+2)
    if t == 1:
        return (1-math.exp(-pow(x/(_5ht+1),(_5ht+1))))*(_5ht+2+_5HTR2_OFFSET)
    if t == 2:
        return (1-math.exp(-pow(x/(_5ht+2+DRN_OFFSET),(_5ht+1+DRN_OFFSET))))*(_5ht+2+DRN_OFFSET)

    

def doExperiment(delay,reward_unseen_speed,speed_uncertainty,reportfile,resultsfile):
    # running the script
    parameters = "delay={},reward_unseen_speed={},speed_uncertainty={}".format(delay,reward_unseen_speed,speed_uncertainty)
    print("Running experiment with: "+parameters)
    reportfile.write("SCRIPT RUN:\n")
    try:
        result = "SUCCESS:\n"+ subprocess.check_output(["prism","final_5ht.prism","prop2_new.pctl",
                                                        "-const",parameters], stderr=subprocess.STDOUT).decode("ISO-8859-1")
    except subprocess.CalledProcessError as e:
        result = "CRASH!!:\n"+ e.output.decode("ISO-8859-1")
        return
    print(result)
    reportfile.write(result)
    reportfile.write("\n")
    print("\n")
    reportfile.write("\n\n\n\n")
    result_in_lines = result.splitlines()
    for line in result_in_lines:
        if 'Result:' in line:
            rr = re.findall("[-+]?[.]?[\d]+(?:,\d\d\d)*[\.]?\d*(?:[eE][-+]?\d+)?", line)
            rr = float(rr[0])
            resultsfile.write(str(rr)+"\n")
            print("p = {}\n\n".format(p))
            return
        

reportfile = open(logfilename, 'wt')
resultsfile = open(pval_filename, 'wt')

print("1 - Normal")
reward_unseen_speed = 100 * ofc5HTreceptors(3, 1.0, 1)
doExperiment(15,reward_unseen_speed,0,reportfile,resultsfile)

print("2 - Less delay")
doExperiment(10,reward_unseen_speed,0,reportfile,resultsfile)

print("3 - DRN is suppressed")
reward_unseen_speed = 100 * ofc5HTreceptors(0.3, 1.0, 1)
doExperiment(15,reward_unseen_speed,0,reportfile,resultsfile)

print("4 - DRN is suppressed, less delay")
doExperiment(15,reward_unseen_speed,0,reportfile,resultsfile)

print("5 - DRN is suppressed and SSRI")
reward_unseen_speed = 100 * ofc5HTreceptors(0.3, 1.0, 3)
doExperiment(15,reward_unseen_speed,0,reportfile,resultsfile)

print("6 - DRN is suppressed and SSRI, less delay")
doExperiment(15,reward_unseen_speed,0,reportfile,resultsfile)

print("7 - DRN is suppressed and 5HTR2 up")
reward_unseen_speed = 100 * ofc5HTreceptors(0.3, 1.0, 2)
doExperiment(15,reward_unseen_speed,0,reportfile,resultsfile)

print("8 - DRN is suppressed and 5HTR2 up, less delay")
doExperiment(15,reward_unseen_speed,0,reportfile,resultsfile)

reportfile.flush()
reportfile.close()

resultsfile.flush()
resultsfile.close()
