# Minimal Makefile to build cpu-shader from sources in src/

CXX := clang++
CXXFLAGS := -std=c++23 -O3 -Wall -Wextra -I src -I .
SRCS := $(wildcard src/*.cpp)
TARGET := cpu-shader

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	-rm -f $(TARGET)

