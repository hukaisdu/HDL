OPT = -std=c++17 -O3 -m64 
LIB = -lpthread -lm

all: HDL HDL_conditional
	
HDL: HDL.cpp
	g++ HDL.cpp -o HDL $(OPT) $(LIB)

HDL_conditional: HDL_conditional.cpp
	g++ HDL_conditional.cpp -o HDL_conditional $(OPT) $(LIB)

.PHONY: clean
clean:
	rm -f HDL HDL_conditional *.o
