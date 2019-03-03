import os
import subprocess
import csv
import re
from pathlib import Path

currentdir = os.getcwd()
logfilename = "results_test.txt"

def doExperiment(delay,reward_unseen_speed,speed_uncertainty,reportfile):
    # running the script
    parameters = "delay={},reward_unseen_speed={},speed_uncertainty={}".format(delay,reward_unseen_speed,speed_uncertainty)
    reportfile.write("SCRIPT RUN:\n")
    try:
        result = "SUCCESS:\n"+ subprocess.check_output(["prism","final_5ht.prism","prop2_new.pctl",
                                                        "-const",parameters], stderr=subprocess.STDOUT).decode("ISO-8859-1")
    except subprocess.CalledProcessError as e:
        result = "CRASH!!:\n"+ e.output.decode("ISO-8859-1")
        return 0
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
            return rr

reportfile = open(logfilename, 'wt')
p = doExperiment(15,2,0,reportfile)
print("p = {}\n".format(p))
reportfile.flush()
reportfile.close()
