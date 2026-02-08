# simple makefile for undefined-runner
# probably going to grow later

TARGET = undefined-runner
SRC    = main.cpp

CXX = g++
STD = -std=c++20

# raylib (system install)
LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# warnings
# keeping these on because they already caught a few bugs
WARNINGS = -Wall -Wextra -Wpedantic -Wshadow

# build modes
DEBUG_FLAGS   = -g -O0 -DDEBUG
RELEASE_FLAGS = -O2 -DNDEBUG

CXXFLAGS = $(STD) $(WARNINGS) $(RELEASE_FLAGS)

.PHONY: all debug run clean fast

all: $(TARGET)

debug:
	$(CXX) $(STD) $(WARNINGS) $(DEBUG_FLAGS) $(SRC) -o $(TARGET) $(LIBS)

# normal build
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $< -o $@ $(LIBS)

run: $(TARGET)
	./$(TARGET)

# quick build when experimenting
fast:
	$(CXX) $(STD) $(SRC) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)

