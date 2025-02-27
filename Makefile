
CXX = g++

CXXFLAGS = -std=c++11 -Wall

TARGET = main

SRC = main.cpp

OBJ = main.o

all: $(TARGET)

use_eur: CXXFLAGS += -DUSE_EUR
use_eur: main

main: main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o main

clean:
	rm -f $(OBJ) $(TARGET)
