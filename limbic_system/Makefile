OPT= -O2 -Wall -g -march=native
all: demo

filter.o: filter.cpp filter.h
	g++ $(OPT) -c filter.cpp

demo.o: demo.cpp limbic_system.o
	g++ $(OPT) -c demo.cpp

demo: demo.o liblimbic_system.a
	g++ demo.o -L. -llimbic_system -o demo
	chmod a+x demo

clean:
	rm -f *.o demo *.dat *~ liblimbic_system.a
