#include <stdio.h>
#include <stdlib.h>
#include "mythreads.h"

#define NUMCPU 8
#define MAX 10000000

typedef struct __counter_t 
{
	long int value;
} counter_t;

void init(counter_t *c)
{
	c->value = 0;
}

void increment(counter_t *c)
{
	c->value++;
}

void decrement(counter_t *c)
{
	c->value--;
}

int get(counter_t *c)
{
	return c->value;
}

void* thread_increment(void *arg)
{
	for(int i = 0; i < MAX; i++)
	{
		increment((counter_t *) arg);
	}
	return NULL;
}

counter_t c_t;

int main(int argc, char **argv)
{
	init(&c_t);
	pthread_t pt[NUMCPU];
	for(int i = 0; i < NUMCPU; i++)
	{
		Pthread_create(&pt[i], NULL, thread_increment, &c_t);
	}
	for(int i = 0; i < NUMCPU; i++)
	{
		Pthread_join(pt[i], NULL);
	}
	printf("[*] expected value: %d\n", MAX * NUMCPU);
	printf("[*] real value: %ld\n", c_t.value);
	return 0;
}

