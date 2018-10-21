TARGET = main
CXX = g++
LDFLAGS = -g -Wall
.PHONY: $(TARGET)

$(TARGET): main.cpp StaffManagement.cpp
	$(CXX) $(LDFLAGS) $? -o $@

clean:
	rm -rf *.a *.o *.so $(TARGET)