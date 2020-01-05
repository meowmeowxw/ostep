#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include "mythreads.h"


// If done correctly, each child should print their "before" message
// before either prints their "after" message. Test by adding sleep(1)
// calls in various locations.

// You likely need two semaphores to do this correctly, and some
// other integers to track things.

typedef struct __barrier_t 
{
    // add semaphores and other information here
	pthread_mutex_t lock;
	sem_t s1, mutex;
	int num_th;
} barrier_t;


// the single barrier we are using for this program
barrier_t b;

void barrier_init(barrier_t *b, int num_threads) 
{
	sem_init(&b->s1, 0, 0);
	sem_init(&b->mutex, 0, 1);
	b->num_th = num_threads;
}

void barrier(barrier_t *b) 
{
	/*
	 * With only 2 semaphores and 1 counter there might be an incorrectness
	 * in the if, since is not a protected access. Error both in valgrind 
	 * and in clang. Solution use 3 semaphores, however I will do it in the 
	 * next problem
	 */
	/* nvm I think this is a good approach, since I put the critical section
	 * inside the mutex, but there's no wait there so there will be no deadlock
	 */
	sem_wait(&b->mutex);
	b->num_th--;
	if(b->num_th == 0)
	{
		sem_post(&b->s1);
	}
	sem_post(&b->mutex);
	sem_wait(&b->s1);
	sem_post(&b->s1);
}

//
// XXX: don't change below here (just run it!)
//
typedef struct __tinfo_t 
{
    int thread_id;
} tinfo_t;

void *child(void *arg) 
{
    tinfo_t *t = (tinfo_t *) arg;
    printf("child %3d: before\n", t->thread_id);
    barrier(&b);
    printf("child %3d: after\n", t->thread_id);
    return NULL;
}


// run with a single argument indicating the number of 
// threads you wish to create (1 or more)
int main(int argc, char *argv[]) 
{
    assert(argc == 2);
    int num_threads = atoi(argv[1]);
    assert(num_threads > 0);
	Pthread_mutex_init(&b.lock, NULL);
    pthread_t p[num_threads];
    tinfo_t t[num_threads];

    printf("parent: begin\n");
    barrier_init(&b, num_threads);
    
    int i;
    for (i = 0; i < num_threads; i++)
	{
		t[i].thread_id = i;
		Pthread_create(&p[i], NULL, child, &t[i]);
    }

    for (i = 0; i < num_threads; i++) 
	{
		Pthread_join(p[i], NULL);
	}
    printf("parent: end\n");
    return 0;
}

