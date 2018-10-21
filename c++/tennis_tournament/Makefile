CC = gcc
LDFLAGS = -g -Wall
TARGET = main
.PHONY: $(TARGET)

# $?: dependency
# $@: target

$(TARGET): main.c
	$(CC) $(LDFLAGS) $? -o $@

clean:
	rm -rf *.a *.o *.so $(TARGET)
