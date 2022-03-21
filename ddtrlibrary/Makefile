fifo:
	g++ ddtrLibrary/fifo_access/fifo_main.cpp -o fifo_main
lifo:
	g++ ddtrLibrary/lifo_access/lifo_main.cpp -o lifo_main

random_access:
	g++ ddtrLibrary/random_access/random_access_main.cpp -o random_access_main

simple_storage:
	g++ ddtrLibrary/simple_storage_access/simple_storage_main.cpp -o simple_storage_main

script: script.o
	g++ script.o -o script

script.o: script.cpp
	g++ -c script.cpp

all:
	g++ main.cpp -o main
	
clean:
	rm -f *.o

