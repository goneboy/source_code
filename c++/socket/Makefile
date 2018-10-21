CC = gcc
LDFLAGS = -g -Wall
.PHONY: client server


all: client.c server.c
	$(CC) $(LDFLAGS) $? -o $@

client: client.c
	$(CC) $(LDFLAGS) $? -o $@

server: server.c
	$(CC) $(LDFLAGS) $? -o $@


clean:
	rm -rf client server