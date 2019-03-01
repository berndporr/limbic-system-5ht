#!/usr/bin/python3

import numpy as np
import matplotlib.pyplot as plt
import scipy.signal as signal
import math as math
import re

exp = ['normal','normal_less_wait','drn_suppress','drn_suppress_less_wait','drn_suppress_ssri','drn_suppress_ssri_less_wait','drn_suppress_5ht2up','drn_suppress_5ht2up_less_wait']


average = []

f = open('results.txt','r')
results = f.read().splitlines()

for r in results:
    if 'Result:' in r:
        rr = re.findall("[-+]?[.]?[\d]+(?:,\d\d\d)*[\.]?\d*(?:[eE][-+]?\d+)?", r)
        rr = rr[0]
        print(rr)
        average.append(rr)

index = np.arange(len(average))
height = 0.35
fig, ax = plt.subplots()
rects = ax.barh(index+height*1.1,average,height,align='edge')
ax.set_xlabel('r')
ax.set_title('Relative reward')
ax.set_yticks(index + height)
ax.set_yticklabels(exp)
plt.show()
