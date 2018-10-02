CFLAGS = -Wall -std=c++11 --pedantic-errors -O2
all: program
program: main.o 
	g++ ${CFLAGS} main.o -o program

.PHONY:debug
debug: CFLAGS = -g -Wall --pedantic-errors
debug: program

main.o: main.cpp config.h
	g++ ${CFLAGS} -c main.cpp

clean:
	rm -rf *o program