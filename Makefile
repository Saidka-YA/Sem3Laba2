CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic
TARGET = LABA2
SOURCES = main.cpp $(wildcard src/*.cpp) \
	$(filter-out algorithms/genome.cpp,$(wildcard algorithms/*.cpp)) \
	../Sem3Laba1/json_file.cpp

all: $(TARGET)

$(TARGET): $(SOURCES) $(wildcard headers/*.h) algorithms/genome.cpp ../Sem3Laba1/json_file.h
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean
