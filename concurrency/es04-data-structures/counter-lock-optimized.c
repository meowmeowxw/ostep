#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mythreads.h"
#include <sys/syscall.h>
#include <sys/types.h>

#define NUMCPU 8
#define MAX 100000000

typedef struct __counter_t 
{
	long int		global;			// global counter
	pthread_mutex_t glock;			// lock for global counter
	long int		local[NUMCPU];	// local value -> per cpu
	pthread_mutex_t llock[NUMCPU];	// lock for local values
	int threshold;					// threshold for a local value
} counter_t;

void init(counter_t *c, int threshold)
{
	c->threshold = threshold;						// set threshold
	c->global = 0;
	Pthread_mutex_init(&c->glock, NULL);
	for(int i = 0; i < NUMCPU; i++)
	{
		c->local[i] = 0;
		Pthread_mutex_init(&c->llock[i], NULL);
	}
}

int get(counter_t *c)
{
	Pthread_mutex_lock(&c->glock);
	int rc = c->global;
	Pthread_mutex_unlock(&c->glock);
	return rc;
}

void update(counter_t *c, int cpu)
{
	// no sense to lock the value to increment only one, this is flawed
	Pthread_mutex_lock(&c->llock[cpu]);				// lock the local value
	c->local[cpu] += 1;
	// if the local value is bigger than the threshold, then update
	// the global value. But first lock the glock
	if(c->local[cpu] >= c->threshold)
	{
		Pthread_mutex_lock(&c->glock);
		c->global += c->local[cpu];
		Pthread_mutex_unlock(&c->glock);
		c->local[cpu] = 0;
	}
	Pthread_mutex_unlock(&c->llock[cpu]);
}


void* thread_increment(void *arg)
{
	int cpu = syscall(SYS_gettid) % NUMCPU;
	counter_t *c = (counter_t*)arg;
	Pthread_mutex_lock(&c->llock[cpu]);	
	while(c->local[cpu] < c->threshold)
	{
		c->local[cpu] += 1;
		if(c->local[cpu] >= c->threshold)
		{
			Pthread_mutex_lock(&c->glock);
			c->global += c->local[cpu];
			Pthread_mutex_unlock(&c->glock);
			c->local[cpu] = 0;
			break;
		}
	}
	Pthread_mutex_unlock(&c->llock[cpu]);
	return NULL;
}

/*
void* thread_increment(void *arg)
{
	int cpu = syscall(SYS_gettid) % NUMCPU;
	for(int i = 0; i < MAX; i++)
	{
		update((counter_t*)arg, cpu);
	}
	return NULL;
}
*/
counter_t c_t;

int main(int argc, char **argv)
{
	int threshold = MAX;
	init(&c_t, threshold);
	pthread_t pt[NUMCPU];
	for(int i = 0; i < NUMCPU; i++)
	{
		Pthread_create(&(pt[i]), NULL, thread_increment, &c_t);
	}
	
	for(int i = 0; i < NUMCPU; i++)
	{
		Pthread_join(pt[i], NULL);
	}

	printf("[*] expected value: %d\n", MAX * NUMCPU);
	printf("[*] real value: %ld\n", c_t.global);
	return 0;
}

