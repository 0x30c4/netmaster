CC=gcc
CFLAGS=-Wall -O2 -c 

INCLUDE=include
SRC=src
BIN=bin
OBJ=obj
EXE=netmaster

DEBUG=debuging

run:
	./$(BIN)/$(EXE)

%.o: $(SRC)/%.c 
	$(CC) $(CFLAGS) $^ -o $(OBJ)/$@

buildtest: clean debug.o common.o err.o handler.o headerparser.o sender.o stringlib.o
	$(CC) $(OBJ)/debug.o $(OBJ)/common.o $(OBJ)/err.o $(OBJ)/handler.o $(OBJ)/headerparser.o $(OBJ)/sender.o $(OBJ)/stringlib.o -o $(BIN)/$(EXE)

runtest: buildtest
	echo -e "\n----------------------------------------------\n"
	./$(BIN)/$(EXE) < $(DEBUG)/header.data

runtest_nc: buildtest
	echo -e "\n----------------------------------------------\n"
	ncat -lvnp 6969 | ./$(BIN)/$(EXE) > /dev/stdout


debug.o: $(DEBUG)/debug.c
	$(CC) $(CFLAGS) -I include $(DEBUG)/debug.c -o $(OBJ)/debug.o

clean:
	rm -fr $(BIN)/* $(OBJ)/*
