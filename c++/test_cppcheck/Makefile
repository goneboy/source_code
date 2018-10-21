CXX = g++
TARGET = main
LDFLAGS = -g -Wall
.PHONY: $(TARGET)

# $?: all dependencies
# $@: target value
$(TARGET): main.cpp
	$(CXX) $(LDFLAGS) $? -o $@

clean:	
	rm -rf *.o *.a *.so $(TARGET)

memcheck:
	valgrind --tool=memcheck --leak-check=yes ./$(TARGET)

cppcheck:
	/root/Desktop/softwares/cppcheck-1.81/cppcheck --enable=warning main.cpp