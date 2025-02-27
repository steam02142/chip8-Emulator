# Compiler and flags
CXX = g++
CXXFLAGS = -g -Wall

# Directories
SRC_DIR = src
BUILD_DIR = build

# Target executable
TARGET = $(BUILD_DIR)/main

# Object files
OBJECTS = $(BUILD_DIR)/main.o $(BUILD_DIR)/chip8.o $(BUILD_DIR)/display.o

# Default rule
all: $(TARGET)

# Rule for the target program
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ -lSDL2

# Compile source files into object files
$(BUILD_DIR)/main.o: $(SRC_DIR)/main.cpp $(SRC_DIR)/chip8.h $(SRC_DIR)/display.h
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/main.cpp -o $@

$(BUILD_DIR)/chip8.o: $(SRC_DIR)/chip8.cpp $(SRC_DIR)/chip8.h
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/chip8.cpp -o $@

$(BUILD_DIR)/display.o: $(SRC_DIR)/display.cpp $(SRC_DIR)/display.h
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/display.cpp -o $@

# Clean rule
clean:
	rm -rf $(BUILD_DIR)
