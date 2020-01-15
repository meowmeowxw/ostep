#include "mythreads.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define _TIME_ 1000

void *child(void *arg) {
    pthread_t pt;
    usleep(_TIME_);
    int id = *(int *)arg;
    printf("received: %d\n", id);
    if (id == 100)
        pthread_exit(NULL);
    id += 1;
    Pthread_create(&pt, NULL, child, &id);
    Pthread_join(pt, NULL);
    pthread_exit(NULL);
}

int main(int argc, char **argv) {
    pthread_t pt;
    int id = 0;
    usleep(1000);
    Pthread_create(&pt, NULL, child, &id);
    Pthread_join(pt, NULL);
    return 0;
}
