CFLAGS=-g -D_THREAD_SAFE -D_REENTRANT -D_POSIX_C_SOURCE=200112L -Wall -Werror\
	   -D_GNU_SOURCE -I../include
LFLAGS=-lpthread -lrt

SRCS	:= ./es01-bridge.c \
	./es02-bridge.c \
	./es01-funicular.c \
	./es02-funicular.c

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

