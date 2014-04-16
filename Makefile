CC = gcc
CFLAGS = -g -ansi -lm -Wall -Wno-unused-function
OBJ = y.tab.o lex.yy.o apex.o util.o vm.o
INSTALL_DIR = /usr/lib/
INCLUDE_DIR = /usr/include
BIN_DIR = /usr/bin
OUT = libapex.so
BIN = apex

all: apex

install: $(OUT)  $(BIN)
	cp $(OUT) $(INSTALL_DIR)
	cp apex.h $(INCLUDE_DIR)
	cp $(BIN) $(BIN_DIR)

deinstall:
	rm -f $(INSTALL_DIR)/$(OUT)
	rm -f $(INCLUDE_DIR)/libapex.h
	rm -f $(BIN_DIR)/$(BIN)

apex: $(OBJ) main.o
	$(CC) -shared $(CFLAGS) $(OBJ) -o $(OUT)
	$(CC) $(CFLAGS) $(OBJ) main.o -o apex

lex: lex.l parse.y
	yacc -y -d parse.y
	lex lex.l

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

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
	rm -f $(OUT)
	rm -f examples/simple

