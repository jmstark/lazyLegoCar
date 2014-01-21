CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=main.cpp communication.cpp observer.cpp pathfinding.cpp pipe.cpp socket.cpp threads.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=lazyLegoCar

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

