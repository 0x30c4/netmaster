CC = gcc
CFLAGS = -Wall -c -g

INCLUDE = include
SRC = src
BIN = bin
OBJ = obj
EXE = netmaster

DEBUG = debuging

# all: master

run:
	./netmaster

bulidtest: debug.o common.o err.o handler.o headerparser.o sender.o stringlib.o
	$(CC) $(OBJ)/debug.o $(OBJ)/common.o $(OBJ)/err.o $(OBJ)/handler.o $(OBJ)/headerparser.o $(OBJ)/sender.o $(OBJ)/stringlib.o -o $(BIN)/$(EXE)

runtest: bulidtest
	./$(BIN)/$(EXE) < $(DEBUG)/header.data

debug.o: $(DEBUG)/debug.c
	$(CC) $(CFLAGS) -I include $(DEBUG)/debug.c -o $(OBJ)/debug.o

common.o: $(SRC)/common.c
	$(CC) $(CFLAGS) $(SRC)/common.c -o $(OBJ)/common.o

err.o: $(SRC)/err.c
	$(CC) $(CFLAGS) $(SRC)/err.c -o $(OBJ)/err.o

handler.o: $(SRC)/handler.c
	$(CC) $(CFLAGS) $(SRC)/handler.c -o $(OBJ)/handler.o

headerparser.o: $(SRC)/headerparser.c
	$(CC) $(CFLAGS) $(SRC)/headerparser.c -o $(OBJ)/headerparser.o


sender.o: $(SRC)/sender.c
	$(CC) $(CFLAGS) $(SRC)/sender.c -o $(OBJ)/sender.o

server.o: $(SRC)/server.c
	$(CC) $(CFLAGS) $(SRC)/server.c -o $(OBJ)/server.o

stringlib.o: $(SRC)/stringlib.c
	$(CC) $(CFLAGS) $(SRC)/stringlib.c -o $(OBJ)/stringlib.o

clean:
	rm -fr $(BIN)/* $(OBJ)/*
