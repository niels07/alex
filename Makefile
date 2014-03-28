CC = gcc
CFLAGS = -g -ansi -lm -Wall -Wno-unused-function
OBJ = y.tab.o lex.yy.o apex.o util.o vm.o
BIN = apex

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(BIN)

lex: lex.l parse.y
	yacc -y -d parse.y
	lex lex.l

y.tab.o: y.tab.h y.tab.c
	$(CC) $(CFLAGS) -c y.tab.c

lex.yy.o: lex.yy.c
	$(CC) $(CFLAGS) -c lex.yy.c

apex.o: apex.h apex.c
	$(CC) $(CFLAGS) -c apex.c

parse.o: parse.h parse.c
	$(CC) $(CFLAGS) -c parse.c

util.o: util.h util.c
	$(CC) $(CFLAGS) -c util.c

vm.o: vm.h vm.c
	$(CC) $(CFLAGS) -c vm.c

clean: 
	rm -f $(OBJ)

