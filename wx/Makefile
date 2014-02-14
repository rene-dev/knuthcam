CC=g++
CFLAGS=-g -c -Wall `pkg-config --cflags --static glfw3` `wx-config --cppflags`
LDFLAGS=`wx-config --libs gl`
SOURCES=main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=KnuthCam

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE) $(EXECUTABLE).app

bundle: $(EXECUTABLE)
	mkdir -p $(EXECUTABLE).app/Contents/MacOS/
	mv $(EXECUTABLE) $(EXECUTABLE).app/Contents/MacOS/