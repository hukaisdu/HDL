OPT= -std=c++17 -O3 -m64 

CC = g++

PHONY: all 

SRC=$(wildcard *.cpp)

OBJ=$(SRC:%.cpp=%.o)

BIN=$(OBJ:%.o=%)

all:$(BIN)

$(BIN):%:%.o
	$(CC) $^ $(OPT) -o $@

$(OBJ):%.o:%.cpp
	$(CC) -c $(OPT) $^ -o $@

clean:
	rm $(OBJ) $(BIN)
