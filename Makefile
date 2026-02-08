# simple makefile for undefined-runner
# kept intentionally straightforward
# this will probably grow as the project grows

TARGET = undefined-runner
SRC    = main.cpp

# compilers
GCC   = g++
CLANG = clang++

STD = -std=c++20

# raylib (system install)
LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# warnings
# keeping these on because they already caught a few bugs
WARNINGS = -Wall -Wextra -Wpedantic -Wshadow

# build modes
DEBUG_FLAGS   = -g -O0 -DDEBUG
RELEASE_FLAGS = -O2 -DNDEBUG

# default flags = release build
CXXFLAGS = $(STD) $(WARNINGS) $(RELEASE_FLAGS)

.PHONY: all gcc clang debug fast run clean

# build both by default
all: gcc clang

# gcc build
gcc: $(TARGET)-gcc

$(TARGET)-gcc: $(SRC)
	$(GCC) $(CXXFLAGS) $< -o $@ $(LIBS)

# clang build
clang: $(TARGET)-llvm

$(TARGET)-llvm: $(SRC)
	$(CLANG) $(CXXFLAGS) $< -o $@ $(LIBS)

# debug build (gcc by default)
debug:
	$(GCC) $(STD) $(WARNINGS) $(DEBUG_FLAGS) $(SRC) -o $(TARGET)-debug $(LIBS)

# quick build when experimenting
fast:
	$(GCC) $(STD) $(SRC) -o $(TARGET)-fast $(LIBS)

# run default gcc build
run: $(TARGET)-gcc
	./$(TARGET)-gcc

clean:
	rm -f \
		$(TARGET)-gcc \
		$(TARGET)-llvm \
		$(TARGET)-debug \
		$(TARGET)-fast

