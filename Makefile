# Compiler
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

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

# Run target (clean + build + execute)
run: clean all
	@if [ -z "$(prog)" ]; then \
		echo "Uso: make run prog=nome [arg=valor]"; \
		exit 1; \
	fi
	./$(BIN_DIR)/$(prog) $(arg)

# Clean build files
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# Phony targets
.PHONY: all clean dirs run