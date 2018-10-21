CC = gcc
LDFLAGS = -g -Wall -std=c99 -lpthread
.PHONY: thread_create thread_join thread_multiwait

all: thread_multiwait 

thread_multiwait: thread_multiwait.c
	$(CC) $(LDFLAGS) $? -o $@

thread_join: thread_join.c
	$(CC) $(LDFLAGS) $? -o $@

thread_create: thread_create.c
	$(CC) $(LDFLAGS) $? -o $@

memcheck:
	valgrind --tool=memcheck --leak-check=yes ./thread_multiwait

clean:
	rm -rf *.o *.a *.so *.out thread_create thread_join thread_multiwait
