#include "lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct __counter_t {
    long int value;
} counter_t;

void init(counter_t *c) { c->value = 0; }

void increment(counter_t *c) { c->value++; }

void decrement(counter_t *c) { c->value--; }

int get(counter_t *c) { return c->value; }

counter_t c_t;

int main(int argc, char **argv) {
    struct timespec start, end;
    long int total_time;
    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < (MAX * NUMCPU); i++) {
        increment(&c_t);
    }
    clock_gettime(CLOCK_REALTIME, &end);
    total_time =
        BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
    printf("[*] expected value: %d\n", MAX * NUMCPU);
    printf("[*] real value: %ld\n", c_t.value);
    printf("[*] time: %ld ns\n", total_time);
    return 0;
}
