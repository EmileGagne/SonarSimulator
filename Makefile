CC=g++
FLAGS=-Wall -O3 -std=c++11

root=$(shell pwd)

test_exec_dir=build/test/bin
test_result_dir=build/test-report
test_work_dir=build/test/work

default: simulateSonar
	echo "Building all"

clean:
	rm -rf build

prepare: clean
	mkdir -p build || true
	mkdir -p build/bin || true
	
simulateSonar: prepare
	$(CC) $(FLAGS) -o build/bin/simulate-sonar src/simulate-sonar.cpp
	
test-quick: default
	mkdir -p $(test_exec_dir)
	$(CC) $(FLAGS) -o $(test_exec_dir)/tests test/main.cpp
	mkdir -p $(test_result_dir)
	mkdir -p $(test_work_dir)
	cd $(test_work_dir)
	$(root)/$(test_exec_dir)/tests || true
