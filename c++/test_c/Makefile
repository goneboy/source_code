TARGET = main
CC = gcc
LDFLAGS = -g -Wall
RM = rm -rf
.PHONY: $(TARGET)

$(TARGET): main.c
	$(CC) $(LDFLAGS) $? -o $@

clean:
	$(RM) *.o *.so *.a $(TARGET)
