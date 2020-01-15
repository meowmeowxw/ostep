#include <mythreads.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define N 50
#define M 10
#define K 5

int display_shavor = 0;
int display_dish = 0;
int ticket_shavor = 0;
int ticket_dish = 0;

pthread_cond_t sync_shavor;
pthread_cond_t sync_dish;
pthread_mutex_t lock_shavor;
pthread_mutex_t lock_dish;

void *legionary(void *arg) {
    int id = *(int *)arg;
    int myticket;
    if (id % 2 == 0) {
        Pthread_mutex_lock(&lock_shavor);
        myticket = ticket_shavor;
        ticket_shavor++;
        printf("legionary shavor: %d\twith ticket: %d\n", id, myticket);
        Pthread_mutex_unlock(&lock_shavor);
        sleep(1);
        Pthread_mutex_lock(&lock_shavor);
        while (myticket != display_shavor) {
            Pthread_cond_wait(&sync_shavor, &lock_shavor);
        }
        // my turn
        printf("legionary shavor: %d\twith ticket: %d\thas done\n", id,
               myticket);
        display_shavor++;
        Pthread_mutex_unlock(&lock_shavor);
        sleep(1);
        Pthread_mutex_lock(&lock_shavor);
        Pthread_cond_broadcast(&sync_shavor);
        Pthread_mutex_unlock(&lock_shavor);
    } else {
        Pthread_mutex_lock(&lock_dish);
        myticket = ticket_dish;
        ticket_dish++;
        printf("legionary dish: %d\twith ticket: %d\n", id, myticket);
        Pthread_mutex_unlock(&lock_dish);
        sleep(1);
        Pthread_mutex_lock(&lock_dish);
        while (myticket != display_dish) {
            Pthread_cond_wait(&sync_dish, &lock_dish);
        }
        // my turn
        display_dish++;
        printf("legionary dish: %d\twith ticket: %d\thas done\n", id, myticket);
        Pthread_mutex_unlock(&lock_dish);
        sleep(1);
        Pthread_mutex_lock(&lock_dish);
        Pthread_cond_broadcast(&sync_dish);
        Pthread_mutex_unlock(&lock_dish);
    }
    return NULL;
}

int main(int argc, char **argv) {
    int i;
    int x[N];
    pthread_t th[N];
    Pthread_mutex_init(&lock_shavor, NULL);
    Pthread_mutex_init(&lock_dish, NULL);
    Pthread_cond_init(&sync_shavor, NULL);
    Pthread_cond_init(&sync_dish, NULL);
    for (i = 0; i < N; i++) {
        x[i] = i;
        Pthread_create(&th[i], NULL, legionary, &x[i]);
    }
    for (i = 0; i < N; i++) {
        Pthread_join(th[i], NULL);
    }
    return 0;
}
