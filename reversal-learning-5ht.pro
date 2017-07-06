TEMPLATE	= app
CONFIG		+= qt warn_on debug
HEADERS		= bandp.h merge2quicktime.h robot.h \
                  defs.h worldpoint.h world.h \
                  reversal-learning-5ht.h direction.h
SOURCES		= bandp.cpp robot.cpp world.cpp merge2quicktime.cpp \
                  reversal-learning-5ht.cpp worldpoint.cpp direction.cpp
TARGET		= reversal-learning-5ht
LIBS            += -lquicktime -L/usr/local/lib -L./limbic_system -llimbic_system
INCLUDEPATH     += limbic_system
INCLUDEPATH     += /usr/include/lqt/
SUBDIRS		= limbic_system
QMAKE_PRE_LINK  = make -C limbic_system
QMAKE_CLEAN	+= limbic_system/liblimbic_system.a
QMAKE_CLEAN	+= limbic_system/limbic_system.o
QMAKE_CLEAN	+= ./limbic_system/direction.o
QMAKE_CLEAN	+= ./limbic_system/bandp.o
QT              += widgets
QMAKE_CXXFLAGS += -std=c++0x -march=native
