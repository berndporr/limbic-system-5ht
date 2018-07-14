#!/usr/bin/python3

import numpy as np
import pylab as pl
#
data = np.loadtxt('log.dat');
#
fig = pl.figure(1)
#
# compound
pl.subplot(611);
pl.plot(data[:,0],data[:,9]);
pl.ylabel('mPFC_LG');
#
pl.subplot(612);
pl.plot(data[:,0],data[:,8]);
pl.ylabel('core w lg2lg');
#
pl.subplot(613);
pl.plot(data[:,0],data[:,10]);
pl.ylabel('mPFC_DG');
#
#
pl.subplot(614);
pl.plot(data[:,0],data[:,11]);
pl.ylabel('core w dg2dg');
#
pl.subplot(615);
pl.plot(data[:,0],data[:,14]);
pl.ylabel('VTA');
#
pl.subplot(616);
pl.plot(data[:,0],data[:,25]);
pl.ylabel('DRN');
#
pl.show();
