#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <mythreads.h>

#define NUM_CAVEMEN 3
#define A 0
#define B 1

pthread_mutex_t lock;
pthread_cond_t full, turn;
int is_full = 0;
int queue = A;

void print_status_dinosaur() {
    char q = queue == A ? 'A' : 'B';
    printf("\ncavemen: %d\ton queue: %c\n", is_full, q);
    puts("move queue to the other side\n");
    fflush(stdout);
}

void print_status_caveman(int id, int myqueue) {
    char q = myqueue == A ? 'A' : 'B'; 
    printf("caveman: %d\tis waiting on: %c\n", id, q);
    fflush(stdout);
}

void *dinosaur(void *arg) {
    while (1) {
        Pthread_mutex_lock(&lock);
        while (is_full != 2) {
            Pthread_cond_wait(&full, &lock);
        }
        print_status_dinosaur();
        is_full = 0;
        queue = (queue + 1) % 2;
        Pthread_cond_broadcast(&turn);
        Pthread_mutex_unlock(&lock);
        sleep(1);
    }
    return NULL;
}

void *caveman(void *arg) {
    int myqueue;
    int id;
    myqueue = *(int *)arg % 2;
    id = *(int *)arg;
    while (1) {
        Pthread_mutex_lock(&lock);
        print_status_caveman(id, myqueue);
        while (myqueue != queue || is_full == 2) {
            Pthread_cond_wait(&turn, &lock);
        }
        is_full++;
        printf("caveman: %d\tget on\n", id);
        myqueue = (myqueue + 1) % 2;
        Pthread_cond_signal(&full);
        Pthread_mutex_unlock(&lock);
        sleep(2);
    }
    return NULL;
}

int main(int argc, char **argv) {
    Pthread_mutex_init(&lock, NULL);
    Pthread_cond_init(&full, NULL);
    Pthread_cond_init(&turn, NULL);
    int i;
    int x[NUM_CAVEMEN];
    pthread_t th[NUM_CAVEMEN];
    pthread_t din;
    for (i = 0; i < NUM_CAVEMEN; i++) {
        x[i] = i;
        Pthread_create(&th[i], NULL, caveman, &x[i]);
    }
    Pthread_create(&din, NULL, dinosaur, NULL);
    for (i = 0; i < NUM_CAVEMEN; i++) {
        Pthread_join(th[i], NULL);
    }
    Pthread_join(din, NULL);
	return 0;
}

