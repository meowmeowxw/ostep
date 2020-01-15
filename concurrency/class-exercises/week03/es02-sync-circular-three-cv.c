#include <mythreads.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define NUM 3

pthread_mutex_t sync_lock;
pthread_cond_t sync_cond;
int turn;

void *child(void *arg) {
    int id = *(int *)arg;
    while (1) {
        Pthread_mutex_lock(&sync_lock);
        while (turn != id) {
            Pthread_cond_wait(&sync_cond, &sync_lock);
        }
        turn = (turn + 1) % NUM;
        printf("id: %d\n", id);
        sleep(1);
        Pthread_cond_broadcast(&sync_cond);
        Pthread_mutex_unlock(&sync_lock);
    }
    return NULL;
}

int main(int argc, char **argv) {
    int i;
    int x[NUM];
    pthread_t th[NUM];
    turn = 0;
    Pthread_mutex_init(&sync_lock, NULL);
    Pthread_cond_init(&sync_cond, NULL);
    for (i = 0; i < NUM; i++) {
        x[i] = i;
        Pthread_create(&th[i], NULL, child, &x[i]);
    }
    for (i = 0; i < NUM; i++) {
        Pthread_join(th[i], NULL);
    }
    return 0;
}
