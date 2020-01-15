CFLAGS=-g

.PHONY: all

all: wish.exe

wish.exe: main.o lib.o
	gcc ${CFLAGS} -o wish.exe main.o lib.o

main.o: main.c lib.h
	gcc ${CFLAGS} -o main.o -c main.c

lib.o: lib.c lib.h
	gcc ${CFLAGS} -o lib.o -c lib.c

