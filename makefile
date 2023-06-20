CC = g++

EXECUTABLE = ATM-Simulator
OBJECTS = ./build/atm.o ./build/main.o

all: create_folders bin/EXECUTABLE

create_folders:
	mkdir -p build bin

bin/EXECUTABLE: $(OBJECTS)
	$(CC) $(OBJECTS) -o ./bin/$(EXECUTABLE) -I ./Lib

./build/main.o: ./build/atm.o ./src/main.cc ./Lib/atm.hpp
	$(CC) ./src/main.cc -c -o ./build/main.o
./build/atm.o: ./src/atm.cc ./Lib/atm.hpp
	$(CC) ./src/atm.cc -c -o ./build/atm.o

.PHONY: clean
clean:
	rm -r ./build