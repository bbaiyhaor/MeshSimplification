.PHONY: all
all: main

main: main.o mesh.o
	g++ main.o mesh.o -o main -std=c++11 -O2
main.o: main.cpp
	g++ -c main.cpp -o main.o -std=c++11 -O2
mesh.o: mesh.cpp point.h matrix.h vec.h edge.h vertex.h mesh.h all.h 
	g++ -c mesh.cpp -o mesh.o -std=c++11 -O2
clean:
	rm *.exe *.o