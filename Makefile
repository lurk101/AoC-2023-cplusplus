CFLAGS=-O3 -std=c++23 -mtune=cortex-a76
LDFLAGS=-lpthread

.PHONY: all clean

all: runall

SOURCES = $(wildcard day*/*.cpp)
OBJECTS = $(patsubst day*/%.cpp, %.o, $(SOURCES))
SOURCES += runall.cpp
OBJECTS += runall.o

runall: runall.o $(OBJECTS) 
	g++ $(LDFLAGS) -o runall $(OBJECTS)

day*/%.o: %.cpp
	g++ $(CFLAGS) $(LDFLAGS) -c $< -o $@

clean:
	rm *.o runall
