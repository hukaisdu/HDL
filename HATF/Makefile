CFLAGS = -O3 -std=c++17
LDFLAGS = -pthread 
CC = g++

target: main main_application

main: main.o ascon.o HATF.o split.o polynomial.o quadratic.o AsconHATF.o xoodoo.o XoodooHATF.o log.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

main_application: main_application.o ascon.o HATF.o split.o polynomial.o quadratic.o AsconHATF.o xoodoo.o XoodooHATF.o log.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

%.o:%.cpp
	$(CC) $(CFLAGS) -c $^ -o $@

.PHONY: clean
clean:
	rm -f *.o main main_application
