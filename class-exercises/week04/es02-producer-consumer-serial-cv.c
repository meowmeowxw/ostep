#include <mythreads.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define NUM_PRODUCERS_A 3
#define NUM_PRODUCERS_B 5
#define NUM_CONSUMERS 10

pthread_mutex_t mutex_done;
pthread_mutex_t mutex_a;
pthread_mutex_t mutex_b;
pthread_mutex_t cond_done;
pthread_cond_t cond_a;
pthread_cond_t cond_b;
uint64_t buffer_a[NUM_PRODUCERS_A];
uint64_t buffer_b[NUM_PRODUCERS_B];
int done;

int get_a() { return buffer_a[0]; }
void *producer_a(void *arg) {
    while (1) {
        ;
    }
    return NULL;
}

void *producer_b(void *arg) {
    while (1) {
        ;
    }
    return NULL;
}

void *consumer(void *arg) {
    int a, b;
    while (1) {
        Pthread_mutex_lock(&mutex_done);
        while (done == 0) {
            Pthread_cond_wait(&cond_done, &mutex_done);
        }
        if (done == 1) {
            Pthread_mutex_lock(&mutex_a);
            a = get_a();
            Pthread_mutex_unlock(&mutex_a);
            Pthread_mutex_lock(&mutex_b);
            b = get_b();
            Pthread_mutex_unlock(&mutex_b);
        } else {
            Pthread_mutex_lock(&mutex_b);
            b = get_b();
            Pthread_mutex_unlock(&mutex_b);
            Pthread_mutex_lock(&mutex_a);
            a = get_a();
            Pthread_mutex_unlock(&mutex_a);
        }
    }
    return NULL;
}

int main(int argc, char **argv) { return 0; }
