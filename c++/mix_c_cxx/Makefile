CC = gcc
CXX = g++
LDFLAGS = -g -Wall
.PHONY: main

all: main

main: main.cxx add.cxx mul.c
	$(CXX) $(LDFLAGS) $? -o $@

clean:
	rm -rf *.a *.o *.so *.out main		
