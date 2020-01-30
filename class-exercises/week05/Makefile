CFLAGS=-g -D_THREAD_SAFE -D_REENTRANT -D_POSIX_C_SOURCE=200112L \
	   -D_GNU_SOURCE -I../include
LFLAGS=-lpthread -lrt

SRCS	:= ./es01-flinstones.c \
	./es02-flinstones-multi-process.c

OBJS	:= ${SRCS:c=o}
PROGS	:= ${SRCS:c=exe}

.PHONY: all

all: ${PROGS}

${PROGS} : %.exe : %.o Makefile
	gcc $< -o $@ ${LFLAGS}

clean:
	rm -f ${PROGS} ${OBJS}

%.o: %.c Makefile
	gcc ${CFLAGS} -c $<

