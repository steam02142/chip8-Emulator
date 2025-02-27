# Compiler and flags
CXX = g++
CXXFLAGS = -g -Wall

# Directories
SRC_DIR = src
BUILD_DIR = build

# Target executable
TARGET = $(BUILD_DIR)/main

# Object files
OBJECTS = $(BUILD_DIR)/main.o $(BUILD_DIR)/chip8.o

# Default rule
all: $(TARGET)

# Rule for the target program
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile source files into object files
$(BUILD_DIR)/main.o: $(SRC_DIR)/main.cpp $(SRC_DIR)/chip8.h
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/main.cpp -o $@

$(BUILD_DIR)/chip8.o: $(SRC_DIR)/chip8.cpp $(SRC_DIR)/chip8.h
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/chip8.cpp -o $@

# Clean rule
clean:
	rm -rf $(BUILD_DIR)
