TARGET = main
CXX = g++
LDFLAGS = -g -Wall
.PHONY: $(TARGET)

$(TARGET): main.cpp NumberOfDiscIntersection.cpp
	$(CXX) $(LDFLAGS) $? -o $@

clean:
	rm -rf *.a *.o *.so $(TARGET)

memCheck:
	valgrind --tool=memcheck --leak-check=yes ./$(TARGET)

cppCheck:
	/root/Desktop/softwares/cppcheck-1.81/cppcheck --enable=warning . 2> cppcheck.txt
