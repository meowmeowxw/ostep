#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    int *x;
    x = malloc(sizeof(int));
    *x = 10;
    printf("*x: %d\n", *x);
    return 0;
}

/*
 * > valgrind --leak-check=yes -s ./es02-malloc
 * ==14391== Memcheck, a memory error detector
 *==14391== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
 *==14391== Using Valgrind-3.15.0 and LibVEX; rerun with -h for copyright info
 *==14391== Command: ./es02-malloc
 *==14391==
 **x: 10
 *==14391==
 *==14391== HEAP SUMMARY:
 *==14391==     in use at exit: 4 bytes in 1 blocks
 *==14391==   total heap usage: 2 allocs, 1 frees, 1,028 bytes allocated
 *==14391==
 *==14391== 4 bytes in 1 blocks are definitely lost in loss record 1 of 1
 *==14391==    at 0x483877F: malloc (vg_replace_malloc.c:309)
 *==14391==    by 0x109161: main (es02-malloc.c:7)
 *==14391==
 *==14391== LEAK SUMMARY:
 *==14391==    definitely lost: 4 bytes in 1 blocks
 *==14391==    indirectly lost: 0 bytes in 0 blocks
 *==14391==      possibly lost: 0 bytes in 0 blocks
 *==14391==    still reachable: 0 bytes in 0 blocks
 *==14391==         suppressed: 0 bytes in 0 blocks
 *==14391==
 *==14391== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
 */
