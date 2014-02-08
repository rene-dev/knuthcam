CC=clang++
CFLAGS=-g -c -Wall -std=c++11 `pkg-config --cflags --static glfw3`
LDFLAGS=`pkg-config --static --libs glfw3` -ldxflib
SOURCES=knuthcam.cpp dxfparser.cpp contour.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=knuthcam

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -rf *.o $(EXECUTABLE)
