TEMPLATE	= app
CONFIG		+= qt warn_on debug
HEADERS		= bandp.h merge2quicktime.h robot.h \
                  defs.h worldpoint.h world.h \
                  reversal-learning-5ht.h direction.h \
                  limbic_system\filter.h limbic_system\limbic_system.h
SOURCES		= bandp.cpp robot.cpp world.cpp merge2quicktime.cpp \
                  reversal-learning-5ht.cpp worldpoint.cpp direction.cpp \
                  limbic_system\filter.cpp limbic_system\limbic_system.cpp
TARGET		= reversal-learning-5ht
LIBS            += -lquicktime -L/usr/local/lib
INCLUDEPATH     += limbic_system
INCLUDEPATH     += /usr/include/lqt/
SUBDIRS		= limbic_system
QT              += widgets
QMAKE_CXXFLAGS += -std=c++0x -march=native
