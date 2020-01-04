#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <semaphore.h>
#include "mythreads.h"

#define NUM_PRODUCERS 3
#define NUM_CONSUMERS 3

int MAX = 3;
int fill = 0;
int use = 0;
int loop = 20;
int buffer[3];
sem_t empty;
sem_t full;
sem_t mutex;

void put(int value)
{
	buffer[fill] = value;	// f1
	fill = (fill + 1) % MAX;
}

int get()
{
	int tmp = buffer[use];
	use = (use + 1) % MAX;
	return tmp;
}

void* producer(void *arg)
{
	int i;
	for(i = 0; i <= loop; i++)
	{
		sem_wait(&empty);
		sem_wait(&mutex);
		printf("producer %ld\tput: %d\n", \
				syscall(SYS_gettid) % NUM_PRODUCERS, i);
		i != loop ? put(i) : put(-1);
		sem_post(&mutex);
		sem_post(&full);
	}
	return NULL;
}

void* consumer(void *arg)
{
	int tmp = 0;
	while(tmp != -1)
	{
		sem_wait(&full);
		sem_wait(&mutex);
		tmp = get();
		printf("consumer %ld\tget value: %d\n", \
				syscall(SYS_gettid) % NUM_CONSUMERS, tmp);
		printf("buffer: ");
		for(int i = 0; i < MAX; i++)
		{
			printf("%d ", buffer[i]);
		}
		printf("\n");
		sem_post(&mutex);
		sem_post(&empty);
	}
	return NULL;
}

int main(int argc, char **argv)
{
	int i = 0;
	pthread_t cons[NUM_CONSUMERS], prod[NUM_PRODUCERS];
	sem_init(&empty, 0, 1);
	sem_init(&full, 0, 0);
	sem_init(&mutex, 0, 1);
	for(i = 0; i < NUM_CONSUMERS; i++)
	{
		Pthread_create(&cons[i], NULL, consumer, NULL);
	}
	for(i = 0; i < NUM_PRODUCERS; i++)
	{
		Pthread_create(&prod[i], NULL, producer, NULL);
	}
	for(i = 0; i < NUM_CONSUMERS; i++)
	{
		Pthread_join(cons[i], NULL);
	}
	for(i = 0; i < NUM_PRODUCERS; i++)
	{
		Pthread_join(prod[i], NULL);
	}
	return 0;
}

