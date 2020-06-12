CC = gcc
CFLAGS = -Wall -c -g

all: output rmdotos

run: 
	./tcps

output: master.o server.o parser.o err.o handler.o sender.o common.o
	$(CC) -pthread master.o server.o parser.o err.o handler.o sender.o common.o -o tcps

# output: worker.o server.o parser.o err.o handler.o sender.o common.o
# 	$(CC) -pthread worker.o server.o parser.o err.o handler.o sender.o common.o -o tcps

worker.o: worker.c
	$(CC) $(CFLAGS) -Iinclude worker.c

master.o: master.c
	$(CC) $(CFLAGS) -Iinclude master.c

server.o: include/server.c include/server.h
	$(CC) $(CFLAGS) include/server.c

err.o: include/err.c include/err.h
	$(CC) $(CFLAGS) include/err.c

parser.o: include/parser.c include/parser.h
	$(CC) $(CFLAGS) include/parser.c

handler.o: include/handler.c include/handler.h
	$(CC) $(CFLAGS) include/handler.c

sender.o: include/sender.c include/sender.h
	$(CC) $(CFLAGS) include/sender.c

common.o: include/common.c include/common.h
	$(CC) $(CFLAGS) include/common.c

clean:
	rm -rf *.o tcps

rmdotos:
	rm -fr *.o