CFLAGS=-Wall -Werror -g -D_THREAD_SAFE -D_REENTRANT -D_POSIX_C_SOURCE=200112L \
	   -D_GNU_SOURCE -I../include
LFLAGS=-lpthread

SRCS	:= ./es01-cv.c \
	./es01-semaphore.c \
	./es02-sync-circular-three-semaphore.c \
	./es02-sync-circular-three-cv.c \
	./es03-baker-semaphore.c \
	./es03-baker-cv.c

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

