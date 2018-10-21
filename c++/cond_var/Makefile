CC = gcc
LDFLAGS = -g -Wall -std=c99 -lpthread
.PHONY: cond_var 

all: cond_var

cond_var: cond_var.c
	$(CC) $(LDFLAGS) $? -o $@

clean:
	rm -rf *.o *.a *.so *.out cond_var
