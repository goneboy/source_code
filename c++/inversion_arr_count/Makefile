CXX = g++
LDFLAGS = -g -Wall
TARGET = main
.PHONY: $(TARGET)

$(TARGET): main.cpp InversionCount.cpp
	$(CXX) $(LDFLAGS) $? -o $@

memCheck:
	valgrind --tool=memcheck --leak-check=yes ./$(TARGET)

cppCheck:
	/root/Desktop/softwares/cppcheck-1.81/cppcheck --enable=warning . 2> cppcheck.txt

clean:
	rm -rf *.a *.o *.so $(TARGET)