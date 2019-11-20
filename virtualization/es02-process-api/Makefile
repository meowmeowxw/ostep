SHELL=/bin/bash

all: es08.exe es07.exe es06.exe es05.exe es04.exe es02.exe es01.exe example.exe

es08.exe: es08.c
	gcc -o es08.exe es08.c

es07.exe: es07.c
	gcc -o es07.exe es07.c

es06.exe: es06.c
	gcc -o es06.exe es06.c

es05.exe: es05.c
	gcc -o es05.exe es05.c

es04.exe: es04.c
	gcc -o es04.exe es04.c

es02.exe: es02.c test02.txt
	gcc -o es02.exe es02.c

test02.txt:
	if [[ ! -f test02.txt ]]; then touch test02.txt; fi

es01.exe: es01.c
	gcc -o es01.exe es01.c

example.exe: example.c
	gcc -o example.exe example.c

clean:
	rm es08.exe es07.exe es06.exe es05.exe es04.exe es02.exe test02.txt \
		es01.exe example.exe

