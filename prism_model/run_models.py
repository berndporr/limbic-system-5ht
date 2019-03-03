import os
import subprocess
import csv
from pathlib import Path

currentdir = os.getcwd()

def doTesting(reportfile):
    # running the script
    reportfile.write("SCRIPT RUN:\n")
    try:
        result = "SUCCESS:\n"+ subprocess.check_output(["python3","realtime_iir_main.py"], stderr=subprocess.STDOUT).decode("ISO-8859-1")
    except subprocess.CalledProcessError as e:
        result = "CRASH!!:\n"+ e.output.decode("ISO-8859-1")
    print(result)
    reportfile.write(result)
    reportfile.write("\n")
    print("\n")
    reportfile.write("\n\n\n\n")

reportfile = open(logfile, 'wt')
doTesting(reportfile)
reportfile.flush()
