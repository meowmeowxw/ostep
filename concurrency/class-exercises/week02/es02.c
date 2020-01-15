#include "mythreads.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define NUM 3

int T[NUM] = {0, 0, 0};
int N[NUM] = {0, 0, 0};
pthread_mutex_t lock[NUM];

void *do_deposit(void *arg) {
    int id = *(int *)arg;
    printf("id: %d\n", id);
    while (1) {
        sleep(1);
        Pthread_mutex_lock(&lock[id]);
        T[id] += 10;
        N[id]++;
        Pthread_mutex_unlock(&lock[id]);
    }
    free(arg);
    pthread_exit(NULL);
}

void *do_withdrawal(void *arg) {
    int id = *(int *)arg;
    while (1) {
        sleep(1);
        Pthread_mutex_lock(&lock[id]);
        T[id] -= 9;
        N[id]++;
        Pthread_mutex_unlock(&lock[id]);
    }
    free(arg);
    pthread_exit(NULL);
}

void *italybank(void *arg) {
    int i;
    while (1) {
        sleep(5);
        for (i = 0; i < NUM; i++) {
            Pthread_mutex_lock(&lock[i]);
            printf("N[%d]: %d\n", i, N[i]);
            printf("T[%d]: %d\n", i, T[i]);
            Pthread_mutex_unlock(&lock[i]);
        }
    }
    pthread_exit(NULL);
}

int main(int argc, char **argv) {
    int i, j;
    int *x;
    int deposit = 5;
    int withdrawal = 4;
    pthread_t pt;
    for (i = 0; i < NUM; i++) {
        Pthread_mutex_init(&lock[i], NULL);
        for (j = 0; j < deposit; j++) {
            x = Malloc(sizeof(int));
            *x = i;
            Pthread_create(&pt, NULL, do_deposit, x);
        }
        for (j = 0; j < withdrawal; j++) {
            x = Malloc(sizeof(int));
            *x = i;
            Pthread_create(&pt, NULL, do_withdrawal, x);
        }
    }
    Pthread_create(&pt, NULL, italybank, NULL);
    pthread_exit(NULL);
    return 0;
}
