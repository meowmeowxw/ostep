#include "mythreads.h"
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

int MAX = 2;
int fill = 0;
int use = 0;
int loop = 20;
int buffer[1];
sem_t empty;
sem_t full;

void put(int value) {
    buffer[fill] = value; // f1
    fill = (fill + 1) % MAX;
}

int get() {
    int tmp = buffer[use];
    use = (use + 1) % MAX;
    return tmp;
}

void *producer(void *arg) {
    int i;
    for (i = 0; i <= loop; i++) {
        sem_wait(&empty);
        puts("producer put");
        i != loop ? put(i) : put(-1);
        sem_post(&full);
    }
    return NULL;
}

void *consumer(void *arg) {
    int tmp = 0;
    while (tmp != -1) {
        sem_wait(&full);
        tmp = get();
        printf("consumer get value: %d\n", tmp);
        sem_post(&empty);
        // printf("%d\n", tmp); error because tmp can change
    }
    return NULL;
}

int main(int argc, char **argv) {
    pthread_t cons, prod;
    sem_init(&empty, 0, 1);
    sem_init(&full, 0, 0);
    Pthread_create(&prod, NULL, producer, NULL);
    Pthread_create(&cons, NULL, consumer, NULL);
    Pthread_join(prod, NULL);
    Pthread_join(cons, NULL);
    return 0;
}
