#!/usr/bin/python3

import numpy as np
import pylab as pl
#
data = np.loadtxt('log.dat');
#
fig = pl.figure(1)
#
# compound
pl.subplot(711);
pl.plot(data[:,0],data[:,9]);
pl.ylabel('mPFC_LG');
#
pl.subplot(712);
pl.plot(data[:,0],data[:,8]);
pl.ylabel('core w lg2lg');
#
pl.subplot(713);
pl.plot(data[:,0],data[:,10]);
pl.ylabel('mPFC_DG');
#
pl.subplot(714);
pl.plot(data[:,0],data[:,11]);
pl.ylabel('core w dg2dg');
#
pl.subplot(715);
pl.plot(data[:,0],data[:,26]);
pl.ylabel('mPFC_explore');
#
pl.subplot(716);
pl.plot(data[:,0],data[:,14]);
pl.ylabel('VTA');
#
pl.subplot(717);
pl.plot(data[:,0],data[:,25]);
pl.ylabel('DRN');
#
pl.show();
