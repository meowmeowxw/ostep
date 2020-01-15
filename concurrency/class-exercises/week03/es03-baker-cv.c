#include <mythreads.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define NUM_CUSTOMERS 10

pthread_mutex_t ticket_lock;
pthread_mutex_t turn_lock;
pthread_cond_t sync_cond;

int ticket = 0;
int display = 0;
int serving = 0;

void shelf(int ticket) {
    printf("serving: %d\n", ticket);
    return;
}

void *customer(void *arg) {
    int myticket;
    while (1) {
        Pthread_mutex_lock(&ticket_lock);
        myticket = ticket;
        ticket++;
        Pthread_mutex_unlock(&ticket_lock);
        Pthread_mutex_lock(&turn_lock);
        while (myticket != display && myticket != display + 1) {
            Pthread_cond_wait(&sync_cond, &turn_lock);
        }
        printf("serving: %d\n", myticket);
        serving++;
        if (serving < 2) {
            display++;
            Pthread_cond_broadcast(&sync_cond);
        }
        Pthread_mutex_unlock(&turn_lock);
        sleep(1);
        Pthread_mutex_lock(&turn_lock);
        if (serving == 2) {
            display++;
            Pthread_cond_broadcast(&sync_cond);
        }
        serving--;
        Pthread_mutex_unlock(&turn_lock);
    }
}

int main(int argc, char **argv) {
    int i;
    pthread_t th[NUM_CUSTOMERS];

    Pthread_mutex_init(&ticket_lock, NULL);
    Pthread_mutex_init(&turn_lock, NULL);
    Pthread_cond_init(&sync_cond, NULL);

    for (i = 0; i < NUM_CUSTOMERS; i++) {
        Pthread_create(&th[i], NULL, customer, NULL);
    }
    for (i = 0; i < NUM_CUSTOMERS; i++) {
        Pthread_join(th[i], NULL);
    }
    return 0;
}
