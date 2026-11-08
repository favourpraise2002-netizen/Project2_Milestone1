# Purpose: Build the portable C++17 campus program.
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -O2 -Iinclude
SOURCES = $(wildcard src/*.cpp)
.PHONY: all clean
all: campus
campus: $(SOURCES) $(wildcard include/*.h)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $@
clean:
	rm -f campus *.o src/*.o
