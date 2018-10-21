CXX = g++
LDFLAGS = -g -Wall -lpthread
.PHONY: pthread_create pthread_multiwait pthread_race Problem3Mutex

Problem3Mutex: Problem3Mutex.cpp
	$(CXX) $(LDFLAGS) $? -o $@

pthread_race: pthread_race.cpp
	$(CXX) $(LDFLAGS) $? -o $@

pthread_multiwait: pthread_multiwait.cpp
	$(CXX) $(LDFLAGS) $? -o $@

pthread_create: pthread_create.cpp
	$(CXX) $(LDFLAGS) $? -o $@


clean:
	rm -rf *.o *.a *.so *.out pthread_create pthread_multiwait pthread_race Problem3Mutex
