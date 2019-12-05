#include <stdio.h>
#include <pthread.h>
#include "mythreads.h"

int balance = 0;
pthread_mutex_t lock;

void* worker(void* arg) {
	Pthread_mutex_lock(&lock);
    balance++; // unprotected access 
	Pthread_mutex_unlock(&lock);
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t p;
    Pthread_create(&p, NULL, worker, NULL);
	// when I increment this value in main it can be incremented
	// in the same moment by worker.
	Pthread_mutex_lock(&lock);
    balance++; // unprotected access
	Pthread_mutex_unlock(&lock);
    Pthread_join(p, NULL);
    return 0;
}

