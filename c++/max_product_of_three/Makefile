CC = gcc
TARGET = main
LDFLAGS = -g -Wall -std=c99
.PHONY: $(TARGET)

$(TARGET): main.c MaxProductOfThree.c
	$(CC) $(LDFLAGS) $? -o $@

clean:
	rm -rf *.a *.o *.so $(TARGET)

memCheck:
	valgrind --tool=memcheck --leak-check=yes ./$(TARGET)