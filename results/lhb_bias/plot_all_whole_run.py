#!/usr/bin/python3

import numpy as np
import pylab as pl
#
data = np.loadtxt('log.dat');
#
fig = pl.figure(1)
#
a = 0
b = 42690
pl.title('Full Reversal run');
# compound
pl.subplot(911);
pl.plot(data[a:b,0],data[a:b,1]*4);
pl.plot(data[a:b,0],data[a:b,2]*1.5);
pl.plot(data[a:b,0],data[a:b,3]*1.5);
pl.plot(data[a:b,0],data[a:b,4]);
pl.plot(data[a:b,0],data[a:b,5]);
pl.plot(data[a:b,0],data[a:b,6]);
pl.plot(data[a:b,0],data[a:b,7]);
pl.ylabel('reward/PF/VIS');
# 
pl.subplot(912);
pl.plot(data[a:b,0],data[a:b,8]);
pl.ylabel('core w lg2lg');
#
pl.subplot(913);
pl.plot(data[a:b,0],data[a:b,10]);
pl.ylabel('core w dg2dg');
#
pl.subplot(914);
pl.plot(data[a:b,0],data[a:b,14]);
pl.ylabel('VTA');
#
pl.subplot(915);
pl.plot(data[a:b,0],data[a:b,17]);
pl.ylabel('shell w pflg');
#
pl.subplot(916);
pl.plot(data[a:b,0],data[a:b,18]);
pl.ylabel('shell w pfdg');
#
pl.subplot(917);
pl.plot(data[a:b,0],data[a:b,19]);
pl.ylabel('dlVP');
#
pl.subplot(918);
pl.plot(data[a:b,0],data[a:b,20]);
pl.ylabel('EP');
#
pl.subplot(919);
pl.plot(data[a:b,0],data[a:b,21]);
pl.ylabel('LHb');
pl.show();
