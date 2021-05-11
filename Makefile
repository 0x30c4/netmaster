CC = gcc
CFLAGS = -Wall -c -g -I include

INCLUDE = include
SRC = src
BIN = bin

all: output rmdotos

run: 
	./

clean:
	rm -rf *.o tcps

rmdotos:
	rm -fr *.o
