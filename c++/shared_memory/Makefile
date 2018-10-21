CXX = g++
LDFLAGS = -g -Wall -lpthread
.PHONY: WriterProcess

# $?: dependencies
# $@: target



ReaderProcess: ReaderProcess.cpp
	$(CXX) $(LDFLAGS) $? -o $@

WriterProcess: WriterProcess.cpp	
	$(CXX) $(LDFLAGS) $? -o $@