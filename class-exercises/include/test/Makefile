CC=gcc
CFLAGS=-g -Wall -Werror -D_THREAD_SAFE -D_REENTRANT -D_POSIX_C_SOURCE=200112L
LFLAGS=-lpthread

all: template-threads.exe

template-threads.exe: template-threads.o mythreads.o
	${CC} ${CFLAGS} -o template-threads.exe template-threads.o mythreads.o \
		${LFLAGS}

template-threads.o:	template-threads.c mythreads.h
	${CC} ${CFLAGS} -o template-threads.o -c template-threads.c

mythreads.o: mythreads.c mythreads.h
	${CC} ${CFLAGS} -o mythreads.o -c mythreads.c

.PHONY: clean

clean:
	rm *.o *.exe

