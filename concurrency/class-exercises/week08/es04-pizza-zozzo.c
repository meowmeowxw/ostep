#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <mythreads.h>

#define N 10
#define NUM_CUSTOMERS 1

int full_desk = N;
pthread_mutex_t lock;
pthread_cond_t empty, full;

void *pizza_man(void *arg)
{
	while(1)
	{
		Pthread_mutex_lock(&lock);
		while(full_desk == N)
		{
			Pthread_cond_wait(&empty, &lock);
		}
		full_desk++;
		printf("pizza man has prepared a new pizza\ttotal: %d\n",full_desk);
		Pthread_mutex_unlock(&lock);
		sleep(1);
		Pthread_mutex_lock(&lock);
		Pthread_cond_broadcast(&full);
		Pthread_mutex_unlock(&lock);
	}
}

void *customer(void *arg)
{
	int id = *(int *)arg;
	while(1)
	{
		Pthread_mutex_lock(&lock);
		while(full_desk == 0)
		{
			Pthread_cond_wait(&full, &lock);
		}
		full_desk--;
		printf("customer: %d is taking pizza\n", id);
		Pthread_cond_signal(&empty);
		Pthread_mutex_unlock(&lock);
		sleep(2);
	}
}

int main(int argc, char **argv)
{
	int i;
	int x[NUM_CUSTOMERS];
	pthread_t pizza, th[NUM_CUSTOMERS];
	Pthread_mutex_init(&lock, NULL);
	Pthread_cond_init(&empty, NULL);
	Pthread_cond_init(&full, NULL);
	Pthread_create(&pizza, NULL, pizza_man, NULL);
	for(i = 0; i < NUM_CUSTOMERS; i++)
	{
		x[i] = i;
		Pthread_create(&th[i], NULL, customer, &x[i]);
	}
	for(i = 0; i < NUM_CUSTOMERS; i++)
	{
		Pthread_join(th[i], NULL);
	}	
	return 0;
}

