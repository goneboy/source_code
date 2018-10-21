CXX = g++
LDFLAGS = -g -Wall -lpthread
.PHONY: pro_consumer 

all: pro_consumer

pro_consumer: pro_consumer.cpp
	$(CXX) $(LDFLAGS) $? -o $@

clean:
	rm -rf *.o *.a *.so *.out pro_consumer
