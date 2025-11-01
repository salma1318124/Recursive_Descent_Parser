CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
TARGET = parser
SOURCES = main.cpp Parser.cpp
HEADERS = Parser.h TokenReader.h
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)
	@echo "Build complete! Executable: $(TARGET)"

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)
	@echo "Clean complete!"

run: $(TARGET)
	./$(TARGET) tokens.txt

help:
	@echo "Available targets:"
	@echo "  make          - Build the parser"
	@echo "  make clean    - Remove build artifacts"
	@echo "  make run      - Build and run with tokens.txt"
	@echo "  make help     - Show this help message"

.PHONY: all clean run help