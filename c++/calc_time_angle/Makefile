CC = gcc
TARGET = main
LDFLAGS = -g -Wall
.PHONY: $(TARGET)
	 
main: calcClockAngle.c
	$(CC) $(LDFLAGS) $? -o $@

clean:
	rm -rf *.a *.o *.so $(TARGET)