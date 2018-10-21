TARGET = main
CXX = g++
LDFLAGS = -g -Wall
.PHONY: $(TARGET)

# $?: dependency value
# $@: target value	

$(TARGET): main.cxx
	$(CXX) $(LDFLAGS) $? -o $@

clean:
	rm -rf *.o *.a *.so $(TARGET)		