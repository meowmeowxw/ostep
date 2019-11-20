SHELL=/bin/bash

all: es04 es02 es01 example

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
	rm es04 es02 test02.txt es01 example
