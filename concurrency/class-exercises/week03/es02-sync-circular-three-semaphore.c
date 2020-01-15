#include <mythreads.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 3

sem_t s1, s2, s3;

void *child_1(void *arg) {
    while (1) {
        sem_wait(&s1);
        puts("child 1");
        sleep(1);
        sem_post(&s2);
    }
    return NULL;
}

void *child_2(void *arg) {
    while (1) {
        sem_wait(&s2);
        puts("child 2");
        sleep(1);
        sem_post(&s3);
    }
    return NULL;
}

void *child_3(void *arg) {
    while (1) {
        sem_wait(&s3);
        puts("child 3");
        sleep(1);
        sem_post(&s1);
    }
}

int main(void) {
    pthread_t pt;
    sem_init(&s1, 0, 1);
    sem_init(&s2, 0, 0);
    sem_init(&s3, 0, 0);
    Pthread_create(&pt, NULL, child_1, NULL);
    Pthread_create(&pt, NULL, child_2, NULL);
    Pthread_create(&pt, NULL, child_3, NULL);
    pthread_exit(NULL);
}
