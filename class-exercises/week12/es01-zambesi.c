// IT'S NOT FINISHED
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <mythreads.h>

#define NUM_LEMUR 7
#define NUM_HIPPOS 2
#define MAX 3
#define A 0
#define B 1

pthread_mutex_t mutex;
pthread_mutex_t mutex_side;
pthread_cond_t empty;
pthread_cond_t full[NUM_HIPPOS];
pthread_cond_t change_side[NUM_HIPPOS];
pthread_cond_t pass;
pthread_cond_t arrived;
pthread_cond_t changed_side[NUM_HIPPOS];
int full_river = 0;
int is_arrived = 0;
int is_full[NUM_HIPPOS] = {3, 3};
int side[NUM_HIPPOS] = {A, A};

void *lemur(void *arg) {
    int id = *(int *)arg;
    int myside = A;
    int myhippo = 0;
    while (1) {
        Pthread_mutex_lock(&mutex);
        while (is_full[0] >= MAX && is_full[1] >= MAX) {
            Pthread_cond_wait(&empty, &mutex);
        }
        if (is_full[0] >= MAX) {
            is_full[1]++;
            myhippo = 1;
            printf("lemur %d get on hippo 1\n", id);
            //Pthread_cond_signal(&full[1]);
        } else {
            is_full[0]++;
            myhippo = 0;
            printf("lemur %d get on hippo 0\n", id);
            //Pthread_cond_signal(&full[0]);
        }
        Pthread_mutex_unlock(&mutex);

        Pthread_mutex_lock(&mutex_side);
        while (myside == side[myhippo]) {
            Pthread_cond_wait(&change_side[myhippo], &mutex_side);
        }
        printf("lemur %d arrived on the other side\n", id);
        myside = (myside + 1) % 2;
        is_arrived++;
        Pthread_cond_signal(&arrived);
        Pthread_mutex_unlock(&mutex_side);

        /*Pthread_mutex_lock(&mutex_side);
        while (side[myhippo] == B) {
            Pthread_cond_wait(&changed_side[myhippo], &mutex_side);
        }
        Pthread_mutex_unlock(&mutex_side);
        sleep(1);
        */
    }
    return NULL;
}

void *hippo(void *arg) {
    int id = *(int *)arg;
    while (1) {
        Pthread_mutex_lock(&mutex);
        is_full[id] = 0;
        Pthread_cond_broadcast(&empty);
        Pthread_mutex_unlock(&mutex);
        sleep(2);
        Pthread_mutex_lock(&mutex);
        printf("hippo %d has %d lemur\n", id, is_full[id]);
        Pthread_mutex_unlock(&mutex);

        Pthread_mutex_lock(&mutex_side); 
        while (full_river == 1) {
            Pthread_cond_wait(&pass, &mutex_side);
        }
        printf("hippo %d change side\n", id);
        sleep(2);
        side[id] = B;
        Pthread_cond_broadcast(&change_side[id]);
        while (is_arrived != 3) {
            Pthread_cond_wait(&arrived, &mutex_side);
        }
        is_arrived = 0;
        full_river = 0;
        printf("hippo %d changed side\n", id);
        Pthread_cond_signal(&pass);
        Pthread_mutex_unlock(&mutex_side);
        sleep(3);
        /*
        Pthread_mutex_lock(&mutex_side);
        side[id] = A;
        Pthread_cond_signal(&changed_side[id]);
        Pthread_mutex_unlock(&mutex_side);
        */
    }
    return NULL;
}

int main(int argc, char **argv) {
    int i;
    int x[NUM_LEMUR];
    int y[NUM_HIPPOS];
    pthread_t lem[NUM_LEMUR];
    pthread_t hip[NUM_LEMUR];
    for (i = 0; i < NUM_LEMUR; i++) {
        x[i] = i;
        Pthread_create(&lem[i], NULL, lemur, &x[i]);
    }
    for (i = 0; i < NUM_HIPPOS; i++) {
        y[i] = i;
        Pthread_create(&hip[i], NULL, hippo, &y[i]);
    }
    for (i = 0; i < NUM_LEMUR; i++) {
        Pthread_join(lem[i], NULL);
    }
    for (i = 0; i < NUM_HIPPOS; i++) {
        Pthread_join(hip[i], NULL);
    }
	return 0;
}

