
CXX = g++

CXXFLAGS = -std=c++11 -Wall

TARGET = main

SRC = main.cpp

OBJ = main.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET)

$(OBJ): $(SRC)
	$(CXX) $(CXXFLAGS) -c $(SRC)

clean:
	rm -f $(OBJ) $(TARGET)
