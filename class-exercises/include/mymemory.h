#include <stdlib.h>
#include <stdio.h>

#ifndef __MALLOC__
#define __MALLOC__
void *Malloc(size_t size) {
    void *ptr;
    ptr = malloc(size);
    if (ptr == NULL) {
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }
    return ptr;
}
#endif // __MALLOC__

#ifndef __CALLOC__
#define __CALLOC__
void *Calloc(size_t nmemb, size_t size) {
    void *ptr;
    ptr = calloc(nmemb, size);
    if (ptr == NULL) {
        fprintf(stderr, "calloc failed\n");
        exit(1);
    }
    return ptr;
}
#endif // __CALLOC__

