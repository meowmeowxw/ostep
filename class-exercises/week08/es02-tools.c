#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <mythreads.h>

#define NUM_S1 2
#define NUM_S2 4
#define NUM_S3 2
#define NUM_L1 2
#define NUM_L2 2

pthread_mutex_t mutex_s1[NUM_S1];
pthread_mutex_t mutex_s2[NUM_S2];
pthread_mutex_t mutex_s3[NUM_S3];
pthread_cond_t cond_s1[NUM_S1];
pthread_cond_t cond_s2[NUM_S2];
pthread_cond_t cond_s3[NUM_S3];

int s1[NUM_S1] = {0, 0};
int s2[NUM_S2] = {0, 0};
int s3[NUM_S3] = {0, 0};
char object[] = {'1', '2', '3'};

void print_take_l(int option, int id, char s) {
    printf("l%d[%d] take tool s%c\n", option, id, s);
}

void print_release_l(int option, int id, char s) {
    printf("l%d[%d] release tool s%c\n", option, id, s);
}

void *l1(void *arg) {
    int id = *(int *)arg;
    while (1) {
        Pthread_mutex_lock(&mutex_s1[id]);
        while (s1[id] != 0) {
            Pthread_cond_wait(&cond_s1[id], &mutex_s1[id]);
        }
        s1[id] = 1;
        print_take_l(1, id, object[0]);
        sleep(1);

        Pthread_mutex_lock(&mutex_s2[id]);
        while (s2[id] != 0) {
            Pthread_cond_wait(&cond_s2[id], &mutex_s2[id]);
        }
        s2[id] = 1;
        print_take_l(1, id, object[1]);
        sleep(2);
        s2[id] = 0;
        print_release_l(1, id, object[1]);
        Pthread_cond_signal(&cond_s2[id]);
        Pthread_mutex_unlock(&mutex_s2[id]);

        Pthread_mutex_lock(&mutex_s3[id]);
        while (s3[id] != 0) {
            Pthread_cond_wait(&cond_s3[id], &mutex_s3[id]);
        }
        s3[id] = 1;
        print_take_l(1, id, object[2]);
        sleep(2);
        s3[id] = 0;
        print_release_l(1, id, object[2]);
        Pthread_cond_signal(&cond_s3[id]);
        Pthread_mutex_unlock(&mutex_s3[id]);
        s1[id] = 0;
        print_release_l(1, id, object[0]);
        Pthread_cond_signal(&cond_s1[id]);
        Pthread_mutex_unlock(&mutex_s1[id]);

        sleep(1);
    }
    return NULL;
}

void *l2(void *arg) {
    int id = *(int *)arg;
    while (1) {
        Pthread_mutex_lock(&mutex_s3[id]);
        while (s3[id] != 0) {
            Pthread_cond_wait(&cond_s3[id], &mutex_s3[id]);
        }
        s3[id] = 1;
        print_take_l(2, id, object[2]);
        sleep(1);
        s3[id] = 0;
        Pthread_cond_signal(&cond_s3[id]);
        Pthread_mutex_unlock(&mutex_s3[id]);
        
        Pthread_mutex_lock(&mutex_s2[id]);
        while (s2[id] != 0) {
            Pthread_cond_wait(&cond_s2[id], &mutex_s2[id]);
        }
        s2[id] = 1;
        print_take_l(2, id, object[1]);
        Pthread_mutex_lock(&mutex_s3[id]);
        while (s3[id] != 0) {
            Pthread_cond_wait(&cond_s3[id], &mutex_s3[id]);
        }
        s3[id] = 1;
        print_take_l(2, id, object[2]);
        sleep(1);
        s3[id] = 0;
        Pthread_cond_signal(&cond_s3[id]);
        Pthread_mutex_unlock(&mutex_s3[id]);
        sleep(1);
        s2[id] = 0;
        print_release_l(2, id, object[1]);
        Pthread_cond_signal(&cond_s2[id]);
        Pthread_mutex_unlock(&mutex_s2[id]);

        Pthread_mutex_lock(&mutex_s1[id]);
        while (s1[id] != 0) {
            Pthread_cond_wait(&cond_s1[id], &mutex_s1[id]);
        }
        s1[id] = 1;
        print_take_l(2, id, object[0]);
        sleep(1);
        s1[id] = 0;
        print_release_l(2, id, object[0]);
        Pthread_cond_signal(&cond_s1[id]);
        Pthread_mutex_unlock(&mutex_s1[id]);
        s3[id] = 0;
        print_release_l(2, id, object[2]);
        Pthread_cond_signal(&cond_s3[id]);
        Pthread_mutex_unlock(&mutex_s3[id]);

        sleep(1);
    }
    return NULL;
}

int main(int argc, char **argv) {
    int i;
    int x[NUM_L1];
    int y[NUM_L2];
    pthread_t th_l1[NUM_L1];
    pthread_t th_l2[NUM_L2];

    for (i = 0; i < NUM_S1; i++) {
        Pthread_mutex_init(&mutex_s1[i], NULL);
        Pthread_cond_init(&cond_s1[i], NULL);
    }
    for (i = 0; i < NUM_S2; i++) {
        Pthread_mutex_init(&mutex_s2[i], NULL);
        Pthread_cond_init(&cond_s2[i], NULL);
    }
    for (i = 0; i < NUM_S3; i++) {
        Pthread_mutex_init(&mutex_s3[i], NULL);
        Pthread_cond_init(&cond_s3[i], NULL);
    }
    for (i = 0; i < NUM_L1; i++) {
        x[i] = i;
        Pthread_create(&th_l1[i], NULL, l1, &x[i]);
    }
    for (i = 0; i < NUM_L2; i++) {
        y[i] = i;
        Pthread_create(&th_l2[i], NULL, l2, &y[i]);
    }
    for (i = 0; i < NUM_L1; i++) {
        Pthread_join(th_l1[i], NULL);
    }
    for (i = 0; i < NUM_L2; i++) {
        Pthread_join(th_l2[i], NULL);
    }
	return 0;
}

