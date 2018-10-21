TARGET = main
CXX = g++
LDFLAGS = -g -Wall
RM = rm -rf
.PHONY: $(TARGET)

$(TARGET): main.cpp add.cpp
	$(CXX) $(LDFLAGS) -I/root/Desktop/programming/c++/test_c $? -o $@

clean:
	$(RM) *.o *.so *.a $(TARGET)
