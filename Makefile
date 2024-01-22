CXX = g++
CXXFLAGS = -std=c++17 -Wall

TARGET = main

all: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(TARGET).cpp

run:
	make all
	clear
	@./$(TARGET)

clean:
	rm -f ./$(TARGET)
