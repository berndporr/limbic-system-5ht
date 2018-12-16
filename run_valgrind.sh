valgrind --log-file=/tmp/valgr.txt --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./limbic-system-simulator -p 0 -n -b -f 1
