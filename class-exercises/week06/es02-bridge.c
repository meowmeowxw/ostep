#include <mythreads.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define NUM_CARS 8
#define A 0
#define B 1

pthread_cond_t ready, turn[2];
pthread_mutex_t mutex;
int ticket[2] = {0, 0};
int ticket_go[2] = {0, 0};
int queue[2] = {0, 0};
char point[2] = {'A', 'B'};
int free_bridge = 0;

void *bridge(void *arg) {
    sleep(2);
    int side;
    int other_side;
    while (1) {
        side = A;
        other_side = B;
        Pthread_mutex_lock(&mutex);
        if (queue[A] < queue[B]) {
            side = B;
            other_side = A;
        }
        while (ticket[side] == ticket_go[side] || free_bridge) {
            Pthread_cond_wait(&ready, &mutex);
        }
        free_bridge = 1; 
        if (queue[side] != 1) {
            queue[side]--;
            ticket_go[side]++;
            printf("\nticket %d on point %c can pass\n",
                ticket_go[side], point[side]);
            Pthread_cond_broadcast(&turn[side]); 
        } else {
            ticket_go[other_side]++;
            queue[other_side]--;
            printf("\nticket %d on point %c can pass\n",
                ticket_go[other_side], point[other_side]);
            Pthread_cond_broadcast(&turn[other_side]);
        }
        Pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void *car(void *arg) {
    int id = *(int *)arg;
    int side = id % 2;
    int other_side = (side + 1) % 2;
    int myticket;
    while (1) {
        Pthread_mutex_lock(&mutex);
        myticket = ++ticket[side];
        printf("car[%d]\ttake ticket %d\tpoint %c\n", id, myticket,
               point[side]);
        queue[side]++;
        while (myticket != ticket_go[side] || !free_bridge) {
            Pthread_cond_wait(&turn[side], &mutex);
        }
        // start
        printf("car[%d] with ticket %d start the bridge from point %c to\
 point %c\n", id, myticket, point[side], point[other_side]);
        sleep(1);

        // half
        printf("car[%d] with ticket %d crossed half the bridge from point %c to\
 point %c\n", id, myticket, point[side], point[other_side]);
        free_bridge = 0;
        Pthread_cond_signal(&ready);
        Pthread_mutex_unlock(&mutex);
        
        sleep(1);
        printf("car[%d] with ticket %d crossed the bridge from point %c to\
 point %c\n", id, myticket, point[side], point[other_side]);
        printf("car[%d] drives around\n\n", id);
        sleep(10 + (side * 2));
    }
    return NULL;
}

int main(int argc, char **argv) {
    setbuf(stdout, NULL);
    int i;
    int x[NUM_CARS];
    int y[2] = {0, 1};
    pthread_t th[NUM_CARS + 2];

    Pthread_cond_init(&ready, NULL);
    Pthread_cond_init(&turn[0], NULL);
    Pthread_cond_init(&turn[1], NULL);
    Pthread_mutex_init(&mutex, NULL);

    Pthread_create(&th[NUM_CARS], NULL, bridge, &y[0]);
    for (i = 0; i < NUM_CARS; i++) {
        x[i] = i;
        Pthread_create(&th[i], NULL, car, &x[i]);
    }
    Pthread_join(th[NUM_CARS], NULL);
    for (i = 0; i < NUM_CARS; i++) {
        Pthread_join(th[i], NULL);
    }
    return 0;
}
