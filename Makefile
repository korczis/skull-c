.phony:
	all

all: 
	make dirs
	make build

clean:
	rm -rf bin
	rm -rf build

dirs:
	mkdir -p bin
	mkdir -p build
	mkdir -p lib

build: dirs
	echo "Building ..."
	cd ./build &&  cmake ../ && make

test: build 
	./build/bin/HelloWorld
