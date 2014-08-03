# This is Makefile

.PHONY: all

bin:
	mkdir -p bin

build:
	echo "This is build ..."
test:
	echo "This is test ..."

# src/apps

hello_world: src/hello_world/main.c
	gcc -o bin/hello_world src/hello_world/main.c

all: bin build

