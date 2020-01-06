CFLAGS=-Wall -Werror -g -D_THREAD_SAFE -D_REENTRANT -D_POSIX_C_SOURCE=200112L -I../include
LFLAGS=-lpthread

SRCS	:= ./es01-cond-signal.c \
	./es02-sync-circular-three-th.c \
	./es03-baker.c

OBJS	:= ${SRCS:c=o}
PROGS	:= ${SRCS:c=exe}

.PHONY: all

all: ${PROGS}

${PROGS} : %.exe : %.o Makefile
	gcc $< -o $@ ${LFLAGS}

clean:
	rm -f ${PROGS} ${OBJS}

%.o: %.c Makefile
	gcc ${CFLAGS} -D_GNU_SOURCE -c $<

