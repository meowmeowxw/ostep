CFLAGS=-g

all: es01-null es02-malloc es03-malloc-100 es04-malloc-uaf es05-use-free

es01-null: es01-null.c
	gcc ${CFLAGS} -o es01-null es01-null.c

es02-malloc: es02-malloc.c
	gcc ${CFLAGS} -o es02-malloc es02-malloc.c

es03-malloc-100: es03-malloc-100.c
	gcc ${CFLAGS} -o es03-malloc-100 es03-malloc-100.c

es04-malloc-uaf: es04-malloc-uaf.c
	gcc ${CFLAGS} -o es04-malloc-uaf es04-malloc-uaf.c

es05-use-free: es05-use-free.c
	gcc ${CFLAGS} -o es05-use-free es05-use-free.c

clean: es02-malloc es01-null es03-malloc-100 es04-malloc-uaf es05-use-free
	rm es02-malloc es01-null es03-malloc-100 es04-malloc-uaf es05-use-free

