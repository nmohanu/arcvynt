
CXX = g++

CXXFLAGS = -std=c++11 -Wall

TARGET = arcvynt

SRC = main.cpp

OBJ = $(TARGET).o

all: $(TARGET)

use_eur: CXXFLAGS += -DUSE_EUR
use_eur: main

main: main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o $(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)
