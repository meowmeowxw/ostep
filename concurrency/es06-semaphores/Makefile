CFLAGS=-Wall -Werror -g
LFLAGS=-lpthread

all: pc-ss.exe pc-multiple-ss.exe

pc-ss.exe: pc-ss.c
	gcc ${CFLAGS} -o pc-ss.exe pc-ss.c ${LFLAGS}

pc-multiple-ss.exe: pc-multiple-ss.o
	gcc -D_GNU_SOURCE -o pc-multiple-ss.exe pc-multiple-ss.o ${LFLAGS}

pc-multiple-ss.o: pc-multiple-ss.c
	gcc ${CFLAGS} -D_GNU_SOURCE -c -o pc-multiple-ss.o pc-multiple-ss.c

.PHONY: clean

clean:
	rm pc-multiple-ss.exe pc-multiple-ss.o pc-ss.exe

