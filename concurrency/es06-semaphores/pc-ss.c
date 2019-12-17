#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include "mythreads.h"

int MAX = 1;
int buffer[1];
int fill = 0;
int use = 0;
int loop = 20;
sem_t empty;
sem_t full;

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
	for(i = 0; i <= loop * 3; i++)
	{
		sem_wait(&empty);
		puts("producer put");
		/*
		if(i == loop)
		{
			put(-1);
		}else
		{
		*/
			put(i);
		//}
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
		puts("consumer get");
		tmp = get();
		printf("value: %d\n", tmp);
		sem_post(&empty);
		// printf("%d\n", tmp); error because tmp can change
	}
	return NULL;
}

int main(int argc, char **argv)
{
	pthread_t cons, prod;
	sem_init(&empty, 0, 1);
	sem_init(&full, 0, 0);
	Pthread_create(&prod, NULL, producer, NULL);
	Pthread_create(&cons, NULL, consumer, NULL);
	Pthread_join(prod, NULL);
	Pthread_join(cons, NULL);
	return 0;
}

