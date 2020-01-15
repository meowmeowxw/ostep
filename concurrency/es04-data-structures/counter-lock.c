#include "lib.h"
#include "mythreads.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct __counter_t {
    long int value;
    pthread_mutex_t lock;
} counter_t;

void init(counter_t *c) {
    c->value = 0;
    Pthread_mutex_init(&c->lock, NULL);
}

void increment(counter_t *c) {
    Pthread_mutex_lock(&c->lock);
    c->value++;
    Pthread_mutex_unlock(&c->lock);
}

void decrement(counter_t *c) {
    Pthread_mutex_lock(&c->lock);
    c->value--;
    Pthread_mutex_unlock(&c->lock);
}

int get(counter_t *c) {
    Pthread_mutex_lock(&c->lock);
    int rc = c->value;
    Pthread_mutex_unlock(&c->lock);
    return rc;
}

void *thread_increment(void *arg) {
    for (int i = 0; i < MAX; i++) {
        increment((counter_t *)arg);
    }
    return NULL;
}

counter_t c_t;

int main(int argc, char **argv) {
    struct timespec start, end;
    long int total_time;
    init(&c_t);
    pthread_t pt[NUMCPU];
    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < NUMCPU; i++) {
        Pthread_create(&pt[i], NULL, thread_increment, &c_t);
    }
    for (int i = 0; i < NUMCPU; i++) {
        Pthread_join(pt[i], NULL);
    }
    clock_gettime(CLOCK_REALTIME, &end);
    total_time =
        BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
    printf("[*] expected value: %d\n", MAX * NUMCPU);
    printf("[*] real value: %ld\n", c_t.value);
    printf("[*] time: %ld ns\n", total_time);
    return 0;
}
