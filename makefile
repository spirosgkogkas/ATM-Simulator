CC = g++

OBJECTS = ./build/atm.o ./build/main.o

<<<<<<< HEAD
all: create_folders bin/main

create_folders:
	mkdir -p build bin
=======
all: create_build bin/main

create_build:
	mkdir -p build
>>>>>>> 8b5927c45d96e87e13b5380d2300afe3216bb12c

bin/main: $(OBJECTS)
	$(CC) $(OBJECTS) -o ./bin/main -I ./Lib

./build/main.o: ./build/atm.o ./src/main.cc ./Lib/atm.hpp
	$(CC) ./src/main.cc -c -o ./build/main.o
./build/atm.o: ./src/atm.cc ./Lib/atm.hpp
	$(CC) ./src/atm.cc -c -o ./build/atm.o

.PHONY: clean
clean:
	rm -r ./build