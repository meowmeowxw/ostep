#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <mythreads.h>

#define NUM_CARS 8
#define A 0
#define B 1

pthread_cond_t ready, turn[2];
pthread_mutex_t mutex_ticket, mutex_queue;
int ticket[2] = {0, 0};
int ticket_go[2] = {0, 0};
int queue[2] = {0, 0};
char point[2] = {'A', 'B'};
int free_bridge = 0;

void *bridge(void *arg) {
    setbuf(stdout, NULL);
    sleep(2);
    int side;
    while(1) {
        side = A;
        Pthread_mutex_lock(&mutex_queue);
        if (queue[A] < queue[B]) {
            side = B;
        }
        while (ticket[side] == ticket_go[side] || free_bridge) {
            Pthread_cond_wait(&ready, &mutex_queue);
        }
        ticket_go[side]++;
        queue[side]--;
        printf("\nticket %d on point %c can pass\n", ticket_go[side], 
                point[side]);
        free_bridge = 1;
        Pthread_cond_broadcast(&turn[side]);
        Pthread_mutex_unlock(&mutex_queue);
    }
    return NULL;
}

void *car(void *arg) {
    setbuf(stdout, NULL);
    int id = *(int *)arg;
    int side = id % 2;
    int myticket;
    while(1) {
        Pthread_mutex_lock(&mutex_queue);
        myticket = ++ticket[side];
        printf("car[%d]\ttake ticket %d\tpoint %c\n", id, myticket, 
                point[side]);
        //Pthread_mutex_unlock(&mutex_ticket);

        //Pthread_mutex_lock(&mutex_queue);
        queue[side]++;
        while (myticket != ticket_go[side] || !free_bridge) {
            Pthread_cond_wait(&turn[side], &mutex_queue); 
        }
        free_bridge = 0;
        printf("car[%d] with ticket %d crosses the bridge from point %c to\
point %c\n", id, myticket, point[side], point[(side + 1) % 2]);
        sleep(1);
        Pthread_cond_signal(&ready);
        Pthread_mutex_unlock(&mutex_queue);
        side = (side + 1) % 2;
        printf("car[%d] drives around\n\n", id);
        sleep(3);
    }
    return NULL;
}

int main(int argc, char **argv) {
    setbuf(stdout, NULL);
    int i;
    int *x = malloc(sizeof(int) * NUM_CARS);
    assert (x != NULL);
    pthread_t th[NUM_CARS + 1];
    
    Pthread_cond_init(&ready, NULL);
    Pthread_cond_init(&turn[0], NULL);
    Pthread_cond_init(&turn[1], NULL);
    Pthread_mutex_init(&mutex_queue, NULL);
    Pthread_mutex_init(&mutex_ticket, NULL);

    Pthread_create(&th[NUM_CARS], NULL, bridge, NULL);
    for(i = 0; i < 8; i++) {
        x[i] = i;
        Pthread_create(&th[i], NULL, car, &x[i]);
    }
    Pthread_join(th[NUM_CARS], NULL);
    for(i = 0; i < 8; i++) {
        Pthread_join(th[i], NULL);
    }
    free(x);
	return 0;
}

