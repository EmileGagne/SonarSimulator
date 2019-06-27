CC=g++
FLAGS=-Wall -O3 -std=c++11

default: prepare
	echo "Building all"

clean:
	rm -rf build

prepare: clean
	mkdir -p build || true
	mkdir -p build/bin || true
	
simulateSonar: prepare
	$(CC) $(FLAGS) -o build/bin/simulate-sonar src/simulate-sonar.cpp
