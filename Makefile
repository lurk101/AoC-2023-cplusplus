CPPFLAGS=-O3 -std=c++23 -mtune=cortex-a76
LDFLAGS=-lpthread

.PHONY: all clean

SOURCES=$(wildcard day*/*.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))

all: runall

runall: runall.o $(OBJECTS)
	g++ $(LDFLAGS) -o runall runall.o $(OBJECTS)

runall.o: runall.cpp runall.h
	g++ $(CPPFLAGS) -c $< -o $@


day*/%.o: %.cpp runall.h ctpl.h
	g++ $(CPPFLAGS) -c $< -o $@

clean:
	rm -f runall.o runall day*/day*.o
