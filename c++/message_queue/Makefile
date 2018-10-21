CXX = g++
LDFLAGS = -g -Wall
.PHONY: WriterProcess ReaderProcess MessageSend

MessageSend: MessageSend.cpp
	$(CXX) $(LDFLAGS) $? -o $@

ReaderProcess: ReaderProcess.cpp
	$(CXX) $(LDFLAGS) $? -o $@

WriterProcess: WriterProcess.cpp
	$(CXX) $(LDFLAGS) $? -o $@

#all: message_send message_rec

clean:
	rm -rf *.o *.a *.so *.out ReaderProcess WriterProcess
