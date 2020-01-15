#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    int *data, i;
    data = malloc(sizeof(int) * 100);
    free(data);
    for (i = 0; i < 100; ++i) {
        data[i] = i;
    }
    return 0;
}

/*
 * > valgrind --leak-check=yes -s ./es04-malloc-uaf
 *==19547== Memcheck, a memory error detector
 *==19547== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
 *==19547== Using Valgrind-3.15.0 and LibVEX;
 *rerun with -h for copyright info
 *==19547== Command: ./es04-malloc-uaf
 *==19547==
 *==19547== Invalid write of size 4
 *==19547==    at 0x109192: main (es04-malloc-uaf.c:11)
 *==19547==  Address 0x4a49040 is 0 bytes inside a block of size 400 free'd
 *==19547==    at 0x48399AB: free (vg_replace_malloc.c:540)
 *==19547==    by 0x109171: main (es04-malloc-uaf.c:8)
 *==19547==  Block was alloc'd at
 *==19547==    at 0x483877F: malloc (vg_replace_malloc.c:309)
 *==19547==    by 0x109161: main (es04-malloc-uaf.c:7)
 *==19547==
 *==19547==
 *==19547== HEAP SUMMARY:
 *==19547==     in use at exit: 0 bytes in 0 blocks
 *==19547==   total heap usage: 1 allocs, 1 frees, 400 bytes allocated
 *==19547==
 *==19547== All heap blocks were freed -- no leaks are possible
 *==19547==
 *==19547== ERROR SUMMARY: 100 errors from 1 contexts (suppressed: 0 from 0)
 *==19547==
 *==19547== 100 errors in context 1 of 1:
 *==19547== Invalid write of size 4
 *==19547==    at 0x109192: main (es04-malloc-uaf.c:11)
 *==19547==  Address 0x4a49040 is 0 bytes inside a block of size 400 free'd
 *==19547==    at 0x48399AB: free (vg_replace_malloc.c:540)
 *==19547==    by 0x109171: main (es04-malloc-uaf.c:8)
 *==19547==  Block was alloc'd at
 *==19547==    at 0x483877F: malloc (vg_replace_malloc.c:309)
 *==19547==    by 0x109161: main (es04-malloc-uaf.c:7)
 *==19547==
 *==19547== ERROR SUMMARY: 100 errors from 1 contexts (suppressed: 0 from 0)
 */
