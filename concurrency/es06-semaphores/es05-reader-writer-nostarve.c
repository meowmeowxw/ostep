#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <sys/syscall.h>
#include "mythreads.h"

//
// Your code goes in the structure and functions below
//

typedef struct __rwlock_t 
{
	sem_t mutex;
	sem_t writelock;
	sem_t turnstile;
	int readers;
} rwlock_t;


void rwlock_init(rwlock_t *rw) 
{
	rw->readers = 0;
	sem_init(&rw->mutex, 0, 1);
	sem_init(&rw->writelock, 0, 1);
}

void rwlock_acquire_readlock(rwlock_t *rw) 
{
	sem_wait(&rw->mutex);	
	sem_post(&rw->mutex);
	sem_wait(&rw->writelock);
}

void rwlock_release_readlock(rwlock_t *rw) 
{
	sem_post(&rw->writelock);
}

void rwlock_acquire_writelock(rwlock_t *rw) 
{
	sem_wait(&rw->mutex);
	sem_wait(&rw->writelock);
}

void rwlock_release_writelock(rwlock_t *rw) 
{
	sem_post(&rw->mutex);
	sem_post(&rw->writelock);
}


//
// Don't change the code below (just use it!)
// 

int loops;
int value = 0;

rwlock_t lock;

void *reader(void *arg) 
{
    int i;
    for (i = 0; i < loops; i++) 
	{
		rwlock_acquire_readlock(&lock);
		printf("thread: %ld\tread %d\n", syscall(SYS_gettid), value);
		rwlock_release_readlock(&lock);
    }
    return NULL;
}

void *writer(void *arg) 
{
    int i;
    for (i = 0; i < loops; i++) 
	{
		rwlock_acquire_writelock(&lock);
		value++;
		printf("thread: %ld\twrite %d\n", syscall(SYS_gettid), value);
		rwlock_release_writelock(&lock);
    }
    return NULL;
}

int main(int argc, char *argv[]) 
{
    assert(argc == 4);
    int num_readers = atoi(argv[1]);
    int num_writers = atoi(argv[2]);
    loops = atoi(argv[3]);

    pthread_t pr[num_readers], pw[num_writers];

    rwlock_init(&lock);

    printf("begin\n");

    int i;
    for (i = 0; i < num_readers; i++)
		Pthread_create(&pr[i], NULL, reader, NULL);
    for (i = 0; i < num_writers; i++)
		Pthread_create(&pw[i], NULL, writer, NULL);

    for (i = 0; i < num_readers; i++)
		Pthread_join(pr[i], NULL);
    for (i = 0; i < num_writers; i++)
		Pthread_join(pw[i], NULL);

    printf("end: value %d\n", value);

    return 0;
}

