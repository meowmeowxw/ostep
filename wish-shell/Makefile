CFLAGS=-g

.PHONY: all

all: wish.exe

wish.exe: wish.o
	gcc ${CFLAGS} -o wish.exe wish.o

wish.o: wish.c
	gcc ${CFLAGS} -o wish.o -c wish.c

