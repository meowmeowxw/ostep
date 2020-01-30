CFLAGS=-Wall -Werror -g
LFLAGS=-lpthread

all: es01.exe es02.exe

es01.exe: es01.c
	gcc ${CFLAGS} -o es01.exe es01.c ${LFLAGS}

es02.exe: es02.c
	gcc ${CFLAGS} -o es02.exe es02.c ${LFLAGS}

.PHONY: clean

clean:
	rm es01.exe es02.exe

