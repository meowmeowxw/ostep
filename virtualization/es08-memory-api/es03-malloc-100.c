#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	int *data;
	data = malloc(sizeof(int) * 100);
	return 0;
}

/*
 *> valgrind --leak-check=yes -s ./es03-malloc-100
 *==18907== Memcheck, a memory error detector
 *==18907== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
 *==18907== Using Valgrind-3.15.0 and LibVEX; rerun with -h for copyright info
 *==18907== Command: ./es03-malloc-100
 *==18907==
 *==18907==
 *==18907== HEAP SUMMARY:
 *==18907==     in use at exit: 400 bytes in 1 blocks
 *==18907==   total heap usage: 1 allocs, 0 frees, 400 bytes allocated
 *==18907==
 *==18907== 400 bytes in 1 blocks are definitely lost in loss record 1 of 1
 *==18907==    at 0x483877F: malloc (vg_replace_malloc.c:309)
 *==18907==    by 0x109151: main (es03-malloc-100.c:7)
 *==18907==
 *==18907== LEAK SUMMARY:
 *==18907==    definitely lost: 400 bytes in 1 blocks
 *==18907==    indirectly lost: 0 bytes in 0 blocks
 *==18907==      possibly lost: 0 bytes in 0 blocks
 *==18907==    still reachable: 0 bytes in 0 blocks
 *==18907==         suppressed: 0 bytes in 0 blocks
 *==18907==
 *==18907== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
 */

