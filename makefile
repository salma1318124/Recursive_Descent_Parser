# Makefile for C- Language Parser

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
TARGET = parser
SOURCES = main.cpp Parser.cpp
HEADERS = Parser.h TokenReader.h
OBJECTS = $(SOURCES:.cpp=.o)

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)
	@echo "Build complete! Executable: $(TARGET)"

# Compile source files to object files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJECTS) $(TARGET)
	@echo "Clean complete!"

# Run with a test file
run: $(TARGET)
	./$(TARGET) tokens.txt

# Help target
help:
	@echo "Available targets:"
	@echo "  make          - Build the parser"
	@echo "  make clean    - Remove build artifacts"
	@echo "  make run      - Build and run with tokens.txt"
	@echo "  make help     - Show this help message"

.PHONY: all clean run help