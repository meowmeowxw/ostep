#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	int *x;
	x = NULL;
	// segfault
	// gdb: Program received signal SIGSEGV (fault address 0x0)
	printf("x is: %d\n", *x);
	return 0;
}

/*
 * > valgrind --leak-check=yes ./es01-null
 *==11738== Memcheck, a memory error detector
 *==11738== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
 *==11738== Using Valgrind-3.15.0 and LibVEX; rerun with -h for copyright info
 *==11738== Command: ./es01-null
 *==11738==
 *==11738== Invalid read of size 4
 *==11738==    at 0x109154: main (es01-null.c:10)
 *==11738==  Address 0x0 is not stack'd, malloc'd or (recently) free'd
 *==11738==
 *==11738==
 *==11738== Process terminating with default action of signal 11 (SIGSEGV): 
 *dumping
 *core
 *==11738==  Access not within mapped region at address 0x0
 *==11738==    at 0x109154: main (es01-null.c:10)
 *==11738==  If you believe this happened as a result of a stack
 *==11738==  overflow in your program's main thread (unlikely but
 *==11738==  possible), you can try to increase the size of the
 *==11738==  main thread stack using the --main-stacksize= flag.
 *==11738==  The main thread stack size used in this run was 8388608.
 *==11738==
 *==11738== HEAP SUMMARY:
 *==11738==     in use at exit: 0 bytes in 0 blocks
 *==11738==   total heap usage: 0 allocs, 0 frees, 0 bytes allocated
 *==11738==
 *==11738== All heap blocks were freed -- no leaks are possible
 *==11738==
 *==11738== For lists of detected and suppressed errors, rerun with: -s
 *==11738== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
 *[1] 11738 segmentation fault (core dumped)  valgrind --leak-check=yes 
 ./es01-null
*/ 

