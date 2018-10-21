#all: main

# which compiler
CXX = g++

# where are include files
INCLUDE = .

# options for development
LDFLAGS = -g -Wall

#.PHONY: main main.o add.o mul.o

main: add.o mul.o main.o
	$(CXX) $(LDFLAGS) -I$(INCLUDE) $? -o $@
	rm -rf *.o
main.o: main.cpp main.h
	$(CXX) $(LDFLAGS) -I$(INCLUDE) $? -c

add.o: add.cpp main.h add.h
	$(CXX) $(LDFLAGS) -I$(INCLUDE) $? -c

mul.o: mul.cpp add.h mul.h
	$(CXX) $(LDFLAGS) -I$(INCLUDE) $? -c

clean:
	rm -rf *.o *.so *.a main
