#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include "mythreads.h"


//
// Here, you have to write (almost) ALL the code. Oh no!
// How can you show that a thread does not starve
// when attempting to acquire this mutex you build?
//

typedef __ns_mutex_t 
{
	int room1;
	int room2;
	sem_t mutex;
	sem_t s1;
	sem_t s2;
} ns_mutex_t;

void ns_mutex_init(ns_mutex_t *m)
{

}

void ns_mutex_acquire(ns_mutex_t *m)
{
	sem_wait(&m->mutex);
	m->room1++;
	sem_post(&m->mutex);
	sem_wait(&m->s1);
		m->room2++;
		sem_wait(&m->mutex);
		m->room1--;
		if(room1 == 0)
		{
			sem_post(&m->mutex);
			sem_post(&m->s2);
		} else
		{
			sem_post(&m->mutex);
			sem_post(&m->s1);
		}
	sem_wait(&m->s2);
		m->room2--;
}

void ns_mutex_release(ns_mutex_t *m) 
{
	if(m->room2 == 0)
	{
		sem_post(&m->s1);
	} else
	{
		sem_post(&m->s2);
	}
}


void *worker(void *arg) 
{
    return NULL;
}

int main(int argc, char *argv[]) 
{
    printf("parent: begin\n");
    printf("parent: end\n");
    return 0;
}

