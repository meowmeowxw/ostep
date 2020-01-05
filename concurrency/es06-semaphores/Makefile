CFLAGS=-Wall -Werror -g
LFLAGS=-lpthread

SRCS	:= ./pc-ss.c \
	./pc-multiple-ss.c \
	./dining-philosophers.c \
	./es01-fork-join.c \
	./es02-rendezvous.c \
	./es03-barrier.c \
	./es04-reader-writer.c

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

