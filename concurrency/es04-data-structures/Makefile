LFLAGS=-lpthread
CFLAGS=-Wall -Werror -g

all: counter-lock-optimized.exe counter-lock.exe counter-no-lock.exe counter.exe
	
counter-lock-optimized.exe: counter-lock-optimized.c
	gcc ${CFLAGS} -o counter-lock-optimized.exe counter-lock-optimized.c \
		${LFLAGS}

counter-lock.exe: counter-lock.c
	gcc ${CFLAGS} -o counter-lock.exe counter-lock.c ${LFLAGS}

counter-no-lock.exe: counter-no-lock.c
	gcc ${CFLAGS} -o counter-no-lock.exe counter-no-lock.c ${LFLAGS}

counter.exe: counter.c
	gcc ${CFLAGS} -o counter.exe counter.c ${LFLAGS}

.PHONY: clean

clean:
	rm counter-lock.exe counter-no-lock.exe counter.exe counter-lock-optimized.exe

