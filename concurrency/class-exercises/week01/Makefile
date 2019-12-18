CFLAGS=-Wall -Werror -g
LFLAGS=-lpthread

all: es01.exe es02.exe es03.exe es04.exe

es01.exe: es01.c
	gcc ${CFLAGS} -o es01.exe es01.c ${LFLAGS}

es02.exe: es02.c
	gcc ${CFLAGS} -o es02.exe es02.c ${LFLAGS}

es03.exe: es03.c
	gcc ${CFLAGS} -o es03.exe es03.c ${LFLAGS}

es04.exe: es04.c
	gcc ${CFLAGS} -o es04.exe es04.c ${LFLAGS}

.PHONY: clean

clean: 
	rm es04.exe es03.exe es02.exe es01.exe

