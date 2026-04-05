# Compiler
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -Iinclude

# Directories
SRC_DIR = project/src
BUILD_DIR = build
BIN_DIR = bin

# Targets
PIPES_TARGET = $(BIN_DIR)/pipes
THREADS_TARGET = $(BIN_DIR)/threads

# Source files
PIPES_SRC = $(SRC_DIR)/pipes/pipes.cpp
THREADS_SRC = $(SRC_DIR)/threads/threads.cpp

# Default target
all: dirs $(PIPES_TARGET) $(THREADS_TARGET)

# Create necessary directories
dirs:
	mkdir -p $(BUILD_DIR) $(BIN_DIR)

# Build pipes executable
$(PIPES_TARGET): $(PIPES_SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Build threads executable
$(THREADS_TARGET): $(THREADS_SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@ -pthread

# Clean build files
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# Phony targets
.PHONY: all clean dirs