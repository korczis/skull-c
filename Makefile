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
	./build/bin/dns_lookup "google.com" "seznam.cz" "root.cz"
	./build/bin/hello_world "travis"
	./build/bin/http_client http://google.com
	# ./build/bin/icmp_client http://google.com
	./build/bin/interfaces
	./build/bin/thread 6
