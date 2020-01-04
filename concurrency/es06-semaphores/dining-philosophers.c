#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <semaphore.h>
#include "mythreads.h"

#define NUM 5
#define LOOPS 5

sem_t forks[NUM];

int left(int p)
{
	return p;
}

int right(int p)
{
	return (p + 1) % NUM;
}

void get_forks(int p)
{
	// last philosopher
	if(p == NUM - 1)
	{
		sem_wait(&forks[right(p)]);	
		sem_wait(&forks[left(p)]);
	} else
	{
		sem_wait(&forks[left(p)]);
		sem_wait(&forks[right(p)]);
	}
}

void put_forks(int p)
{
	sem_post(&forks[left(p)]);
	sem_post(&forks[right(p)]);
}

void think(int id)
{
	printf("philosopher %d is thinking\n", id);
	sleep(0.5);
	return;
}

void eat(int id)
{
	printf("philsopher %d is eating\n", id);
	sleep(0.5);
	return;
}

void *philosopher(void *arg)
{
	int id = *(int *)arg;
	int i = 0;
	for(i = 0; i < LOOPS; i++)
	{
		think(id);
		get_forks(id);
		eat(id);
		put_forks(id);
	}
	return NULL;
}

int main(int argc, char **argv)
{
	int i = 0;
	int id[NUM];
	pthread_t ph[NUM];
	for(i = 0; i < NUM; i++)
	{
		sem_init(&forks[i], 0, 1);
	}
	for(i = 0; i < NUM; i++)
	{
		id[i] = i;
		Pthread_create(&ph[i], NULL, philosopher, &id[i]);
	}
	for(i = 0; i < NUM; i++)
	{
		Pthread_join(ph[i], NULL);
	}
	puts("philosphers have finished the dinner");
	return 0;
}

