OPT= -O2 -Wall
all: liblimbic_system.a
	make -C isolearning

limbic_system.o: limbic_system.cpp limbic_system.h
	g++ $(OPT) -c limbic_system.cpp

bandp.o: bandp.cpp bandp.h 
	g++ $(OPT) -c bandp.cpp

liblimbic_system.a: limbic_system.o direction.o bandp.o isolearning/isolearning.o isolearning/trace.o
	ar rcs liblimbic_system.a limbic_system.o isolearning/isolearning.o isolearning/trace.o 

demo.o: demo.cpp limbic_system.o
	g++ $(OPT) -c demo.cpp

demo: demo.o liblimbic_system.a
	g++ demo.o -L. -llimbic_system -o demo
	chmod a+x demo

clean:
	rm -f *.o demo *.dat *~ liblimbic_system.a isolearning/*.o
	make -C isolearning clean
