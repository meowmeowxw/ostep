SHELL=/bin/bash

all: es08 es07 es06 es05 es04 es02 es01 example

es08: es08.c
	gcc -o es08 es08.c

es07: es07.c
	gcc -o es07 es07.c

es06: es06.c
	gcc -o es06 es06.c

es05: es05.c
	gcc -o es05 es05.c

es04: es04.c
	gcc -o es04 es04.c

es02: es02.c test02.txt
	gcc -o es02 es02.c

test02.txt:
	if [[ ! -f test02.txt ]]; then touch test02.txt; fi

es01: es01.c
	gcc -o es01 es01.c

example: example.c
	gcc -o example example.c

clean:
	rm es08 es07 es06 es05 es04 es02 test02.txt es01 example

