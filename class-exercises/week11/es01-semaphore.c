#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <mythreads.h>

#define NUM 6
#define GREEN 0
#define YELLOW 1
#define RED 2

pthread_mutex_t mutex_ticket;
pthread_mutex_t mutex;
pthread_cond_t empty;
pthread_cond_t full;
int is_empty = 0;
int is_full = 0;
int ticket = 0;
int ticket_go = 0;
int light = RED;

void *semaphore(void *arg) {
    while (1) {
        sleep(1);
        Pthread_mutex_lock(&mutex);
        light = GREEN;
        ticket_go++;
        Pthread_cond_broadcast(&empty);
        Pthread_mutex_unlock(&mutex);
        sleep(4);

        Pthread_mutex_lock(&mutex);
        light = YELLOW;
        Pthread_mutex_unlock(&mutex);
        sleep(1);

        Pthread_mutex_lock(&mutex);
        light = RED;
        Pthread_mutex_unlock(&mutex);
        sleep(3);
    }
    return NULL;
}

void *car(void *arg) {
    int id = *(int *)arg;
    int myticket;
    while (1) {
        Pthread_mutex_lock(&mutex_ticket);
        myticket = ++ticket;
        printf("car %d take ticket %d\n", id, myticket);
        Pthread_mutex_unlock(&mutex_ticket);

        Pthread_mutex_lock(&mutex);
        while (myticket != ticket_go || light == RED) {
            Pthread_cond_wait(&empty, &mutex);
        }
        if (light == YELLOW) {
            printf("car %d with ticket %d crossing yellow\n", id, myticket);
            printf("ticket %d\t ticket_go %d\n", ticket, ticket_go);
        } else {
            printf("car %d with ticket %d crossing green\n", id, myticket);
            ticket_go++;
            Pthread_cond_broadcast(&empty);
        }
        Pthread_mutex_unlock(&mutex);
        printf("car %d drives around\n", id);
        sleep(3 + id);
    }
    return NULL;
}

int main(int argc, char **argv) {
    int i;
    int x[NUM];
    pthread_t th[NUM];
    pthread_t sem;
    Pthread_create(&sem, NULL, semaphore, NULL);
    for (i = 0; i < NUM; i++) {
        x[i] = i;
        Pthread_create(&th[i], NULL, car, &x[i]);
    }
    Pthread_join(sem, NULL);
    for (i = 0; i < NUM; i++) {
        Pthread_join(th[i], NULL);
    }
	return 0;
}

