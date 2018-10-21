TARGET = main
CXX = g++
LDFLAGS = -g -Wall
.PHONY: $(TARGET)

$(TARGET): main.cpp StringSearch.cpp
	$(CXX) $(LDFLAGS) $? -o $@

memCheck:
	valgrind --leak-check=yes --tool=memcheck ./$(TARGET)

clean:
	rm -rf $(TARGET)
