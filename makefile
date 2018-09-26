CFLAGS = -Wall -std=c++11 --pedantic-errors -O2
all: program
program: main.o mapper.o 
	g++ ${CFLAGS} mapper.o main.o -o program

.PHONY:debug
debug: CFLAGS = -g -Wall --pedantic-errors
debug: program

main.o: main.cpp mapper.h config.h
	g++ ${CFLAGS} -c main.cpp

mapper.o: mapper.h mapper.cpp
	g++ ${CFLAGS} -c mapper.cpp

clean:
	rm -rf *o program