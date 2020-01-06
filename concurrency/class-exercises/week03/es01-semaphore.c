#include <unistd.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <stdint.h>
#include <inttypes.h>
#include <pthread.h> 
#include <semaphore.h>
#include "mythreads.h"

#define SYNC_MAX 10

int sync_count = 0; 
sem_t s[SYNC_MAX];
sem_t mutex;

void barrier() 
{ 
	int id;
	sem_wait(&mutex);
	id = sync_count;
	sync_count++;
	sem_post(&mutex);
	if(id < SYNC_MAX - 1)
	{
		sem_wait(&s[id]);
	}
	else
	{
		sem_post(&s[0]);
		sem_wait(&s[id]);
	}
	sem_post(&s[id + 1]);
} 

void *Thread (void *arg) 
{ 
	//int id = *(int *)arg;
	printf ("I am %lu\n", pthread_self()); 
	barrier();
	printf("I am %lu and I'm leaving\n", pthread_self()); 
	return NULL;
} 

int main () 
{ 
	int i;
	int x[SYNC_MAX];
	pthread_t th[SYNC_MAX]; 
	for(i = 0; i < SYNC_MAX; i++)
	{
		sem_init(&s[i], 0, 0);
	}
	sem_init(&mutex, 0, 1);
	for(i = 0; i < SYNC_MAX; i++) 
	{
		x[i] = i;
		Pthread_create(&th[i], NULL, Thread, &x[i]); 
	}
	for(i = 0; i < SYNC_MAX; i++) 
	{
		Pthread_join(th[i], NULL);
	}
	return 0;
} 

