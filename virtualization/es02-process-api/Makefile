CFLAGS=-g
LFLAGS=-lpthread

SRCS	:= ./es01.c \
	./es02.c \
	./es03.c \
	./es04.c \
	./es04.c \
	./es05.c \
	./es06.c \
	./es07.c \
	./es08.c


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

