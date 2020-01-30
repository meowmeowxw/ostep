#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <mythreads.h>

#define NUM 8

void *child(void *arg) {
    int id = *(int *)arg;
    printf("id: %d\n", id);
    return NULL;
}

int main(int argc, char **argv) {
    int i;
    int x[NUM];
    pthread_t th[NUM];
    for (i = 0; i < NUM; i++) {
        x[i] = i;
        Pthread_create(&th[i], NULL, child, &x[i]);
    }
    for (i = 0; i < NUM; i++) {
        Pthread_join(th[i], NULL);
    }
	return 0;
}

