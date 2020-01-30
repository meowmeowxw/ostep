#include <mythreads.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define NUM_SOBERS 4
#define MAX 2

pthread_mutex_t mutex, mutex_ticket;
pthread_cond_t ready, full, start, end;
int is_full = 0;
int is_empty = 0;
int ticket = 0;
int ticket_go = 0;

void *funicular(void *arg) {
    sleep(1);
    while (1) {
        Pthread_mutex_lock(&mutex);
        while (is_full < MAX) {
            Pthread_cond_wait(&full, &mutex);
        }
        is_empty = MAX;
        puts("\nstart of the panoramic tour");
        sleep(1);
        puts("we're at the top");
        sleep(1);
        puts("tour ended\n");
        Pthread_cond_broadcast(&end);
        // while the funicular is not empty we can't let other passenger get
        // inside
        while (is_empty != 0) {
            Pthread_cond_wait(&start, &mutex);
        }
        // at the end of the tour and when the funicular is empty we can
        // broadcast the status
        is_full = 0;
        Pthread_cond_broadcast(&ready);
        Pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void *sober(void *arg) {
    int id = *(int *)arg;
    int myticket;
    while (1) {
        Pthread_mutex_lock(&mutex_ticket);
        myticket = ticket;
        ticket++;
        printf("passenger %d take ticket %d\n", id, myticket);
        Pthread_mutex_unlock(&mutex_ticket);

        Pthread_mutex_lock(&mutex);
        // wait until is not my turn, or the funicular is full or my ticket
        // is not valid yet
        while (myticket != ticket_go || is_full >= MAX) {
            printf("passenger %d wait...\n", id);
            Pthread_cond_wait(&ready, &mutex);
        }
        is_full++;
        ticket_go++;
        printf("passenger %d with ticket %d get on funicular\n", id, myticket);
        // if the cabine is full then we need to signal the funicular,
        // otherwise, other passengers can enjoy the tour with us and we let
        // them know
        if (is_full == MAX) {
            Pthread_cond_signal(&full);
        } else {
            Pthread_cond_broadcast(&ready);
        }

        // wait until the tour is ended and we signal to the funicular that we
        // got out
        Pthread_cond_wait(&end, &mutex);
        printf("passenger %d left\n", id);
        is_empty--;
        Pthread_cond_signal(&start);
        Pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main(int argc, char **argv) {
    setbuf(stdout, NULL);
    int i;
    int x[NUM_SOBERS];
    pthread_t th[NUM_SOBERS];
    pthread_t fun;
    Pthread_mutex_init(&mutex, NULL);
    Pthread_mutex_init(&mutex_ticket, NULL);
    Pthread_cond_init(&ready, NULL);
    Pthread_cond_init(&full, NULL);
    Pthread_cond_init(&start, NULL);
    Pthread_cond_init(&end, NULL);

    Pthread_create(&fun, NULL, funicular, NULL);
    for (i = 0; i < NUM_SOBERS; i++) {
        x[i] = i;
        Pthread_create(&th[i], NULL, sober, &x[i]);
    }
    Pthread_join(fun, NULL);
    for (i = 0; i < NUM_SOBERS; i++) {
        Pthread_join(th[i], NULL);
    }
    return 0;
}
