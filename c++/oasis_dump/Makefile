################################
# File: Makefile for OASIS tool
# Author: Tran Bao Trung
# Date: 25/10/2017
################################

CXX=g++
TARGET=main
LDFLAGS=-g -Wall
SOURCE=oasisHandle.cxx dumpRecord.cxx oasisLibrary.cxx
.PHONY: $(TARGET)

main:
	$(CXX) $(LDFLAGS) $(SOURCE) -o $(TARGET)
	
clean:
	rm -rf *.o *.a *.so $(TARGET)
	
memCheck:
	valgrind --tool=memcheck --leak-check=yes ./$(TARGET)