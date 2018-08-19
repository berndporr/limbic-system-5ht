#!/usr/bin/python3

import numpy as np
import pylab as pl
#
data = np.loadtxt('log.dat');
#
fig = pl.figure(1)
#
a = 0
b = len(data[:,0])
# compound
pl.subplot(711);
pl.plot(data[a:b,0],data[a:b,9]);
pl.ylabel('mPFC_LG');
#
pl.subplot(712);
pl.plot(data[a:b,0],data[a:b,8]);
pl.ylabel('core w lg2lg');
#
pl.subplot(713);
pl.plot(data[a:b,0],data[a:b,10]);
pl.ylabel('mPFC_DG');
#
pl.subplot(714);
pl.plot(data[a:b,0],data[a:b,11]);
pl.ylabel('core w dg2dg');
#
pl.subplot(715);
pl.plot(data[a:b,0],data[a:b,14]);
pl.ylabel('VTA');
#
pl.subplot(716);
pl.plot(data[a:b,0],data[a:b,25]);
pl.ylabel('DRN');
#
pl.subplot(717);
pl.plot(data[a:b,0],data[a:b,26]);
pl.plot(data[a:b,0],data[a:b,27]);
pl.plot(data[a:b,0],data[a:b,2]*2);
pl.plot(data[a:b,0],data[a:b,3]*2);
pl.ylabel('visR*2/PF');
#
pl.show();
