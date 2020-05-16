CC = gcc
CFLAGS = -Wall -c -g

all: output rmdotos

run: 
	./tcps

output: master.o server.o parser.o err.o
	$(CC) -pthread master.o server.o parser.o err.o -o tcps

master.o: master.c
	$(CC) $(CFLAGS) -Iinclude master.c

server.o: include/server.c include/server.h
	$(CC) $(CFLAGS) include/server.c

err.o: include/err.c include/err.h
	$(CC) $(CFLAGS) include/err.c

parser.o: include/parser.c include/parser.h
	$(CC) $(CFLAGS) include/parser.c

clean:
	rm -rf *.o tcps

rmdotos:
	rm -fr *.o