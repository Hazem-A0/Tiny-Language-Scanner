# Makefile for TINY Language Compiler

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Iinclude
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
DATA_DIR = data

# Target executable
TARGET = tiny_compiler.exe

# Source files
SOURCES = $(SRC_DIR)/cli.cpp

# Object files
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Default target
all: $(TARGET)

# Link
$(TARGET): $(SRC_DIR)/cli.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC_DIR)/cli.cpp

# Create build directory
$(BUILD_DIR):
	mkdir $(BUILD_DIR)

# Clean build artifacts
clean:
	del /Q $(TARGET) 2>nul || true
	del /Q $(DATA_DIR)\*.tree 2>nul || true

# Run tests
test: $(TARGET)
	@echo Running tests...
	@.\$(TARGET) $(DATA_DIR)\input.txt
	@echo.
	@.\$(TARGET) $(DATA_DIR)\factorial.txt
	@echo.
	@.\$(TARGET) $(DATA_DIR)\test_invalid.txt

.PHONY: all clean test
