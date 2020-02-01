#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <mythreads.h>

#define NUM_ALPHA 5
#define NUM_BETA 5
#define ALPHA 0
#define BETA 1

pthread_mutex_t mutex;
pthread_mutex_t mutex_ticket;
pthread_cond_t run[2];
int is_running = 0;
int ticket[2] = {0, 0};
int ticket_go[2] = {0, -1};
const char groups[2] = {'A', 'B'};

void *runner(void *arg) {
    int id = *(int *)arg;
    int group = id % 2;
    int other_group = (group + 1) % 2;
    int myticket;
    while (1) {
        Pthread_mutex_lock(&mutex_ticket);
        myticket = ticket[group]++;
        //printf("runner %d of group %c ticket %d\n", 
        //  id, groups[group], myticket);
        Pthread_mutex_unlock(&mutex_ticket);
         
        Pthread_mutex_lock(&mutex);
        while (myticket != ticket_go[group] || is_running == 1) {
            Pthread_cond_wait(&run[group], &mutex);
        }
        is_running = 1;
        //printf("runner %d of group %c ticket %d runs\n", 
        //  id, groups[group], myticket);
        Pthread_mutex_unlock(&mutex);
        sleep(1);
        Pthread_mutex_lock(&mutex);
        ticket_go[other_group]++;
        is_running = 0;
        printf("runner %d of group %c ticket %d finished\n", 
                id, groups[group], myticket);
        Pthread_cond_broadcast(&run[other_group]);
        Pthread_mutex_unlock(&mutex);
    }
    return NULL;
    free(arg);
}

int main(int argc, char **argv) {
    int i;
    int *x;
    pthread_t th[NUM_ALPHA + NUM_BETA];
    Pthread_mutex_init(&mutex, NULL);
    Pthread_mutex_init(&mutex_ticket, NULL);
    Pthread_cond_init(&run[0], NULL);
    Pthread_cond_init(&run[1], NULL);

    for (i = 0; i < NUM_ALPHA * 2; i += 2) {
        x = malloc(sizeof(int));
        *x = i;
        Pthread_create(&th[i], NULL, runner, x);
    }
    for (i = 1; i < NUM_BETA * 2; i += 2) {
        x = malloc(sizeof(int));
        *x = i;
        Pthread_create(&th[i], NULL, runner, x);
    }
    pthread_exit(NULL);
}

