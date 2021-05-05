CC = gcc
CFLAGS = -Wall -c -g -I /root/CodeZ/C/netmaster/include/

INCLUDE = /root/CodeZ/C/netmaster/include

all: output rmdotos

run: 
	./deb

output: master.o stringlib.o headerparser.o
	$(CC) -I $(INCLUDE) master.o headerparser.o stringlib.o -o deb 2>/dev/stdout


stringlib.o: $(INCLUDE)/stringlib.c
	$(CC) $(CFLAGS) $(INCLUDE)/stringlib.c 2>/dev/stdout

headerparser.o: $(INCLUDE)/headerparser.c
	$(CC) $(CFLAGS) $(INCLUDE)/headerparser.c 2>/dev/stdout

master.o: master.c 
	$(CC) $(CFLAGS) master.c 2>/dev/stdout

clean:
	rm -rf *.o tcps

rmdotos:
	rm -fr *.o
