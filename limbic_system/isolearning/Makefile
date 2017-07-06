OPT= -O2 -g -Wall

all: demo

demo.o: demo.cpp
	g++ $(OPT) -c demo.cpp

isolearning.o: isolearning.cpp isolearning.h
	g++ $(OPT) -c isolearning.cpp

trace.o: trace.cpp trace.h
	g++ $(OPT) -c trace.cpp

libisolearning.a: trace.o isolearning.o
	ar rcs libisolearning.a isolearning.o trace.o

demo: demo.o libisolearning.a
	g++ demo.o -L. -lisolearning -o demo
	chmod a+x demo

clean:
	rm -f demo
	rm -f *.o
	rm -f *.dat
	rm -f *.ps *.eps
	rm -f *~
	rm -f *.a
