#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <semaphore.h>
#include <mythreads.h>

#define N 3
#define M 5
#define K 10
#define A 0
#define B 1

int buffer_a;
int buffer_b;
int option;

sem_t full;
sem_t full_a;
sem_t full_b;

sem_t empty;
sem_t empty_a;
sem_t empty_b;

sem_t mutex_a;
sem_t mutex_b;
sem_t mutex;

void put_a(int a)
{
	buffer_a = a;
	option = A;
}

void put_b(int b)
{
	buffer_b = b;
	option = B;
}

int get_a()
{
	return buffer_a;
}

int get_b()
{
	return buffer_b;
}

int get_option()
{
	return option;
}

void *producer_a(void *arg)
{
	int x;
	for(x = 0; ; x++)
	{
		sem_wait(&empty_a);
		sem_wait(&mutex_a);
		put_a(x);
		sem_post(&mutex_a);
		sem_post(&full_a);
		sem_post(&full);
		sleep(1);
	}
	return NULL;
}

void *producer_b(void *arg)
{
	int x;
	for(x = 0; ; x++)
	{
		sem_wait(&empty_b);
		sem_wait(&mutex_b);
		put_b(x);
		sem_post(&mutex_b);
		sem_post(&full_b);
		sem_post(&full);
		sleep(1);
	}
	return NULL;
}

void *consumer(void *arg)
{
	int id = *(int *)arg;
	int a, b, option;
	while(1)
	{
		sem_wait(&full);
		option = get_option();
		if(option == A)
		{
			// get a
			puts("a");
			sem_wait(&full_a);
			sem_wait(&mutex_a);
			a = get_a();
			sem_post(&mutex_a);
			sem_post(&empty_a);

			// get b
			sem_wait(&full_b);
			sem_wait(&mutex_b);
			b = get_b();
			sem_post(&mutex_b);
			sem_post(&empty_b);
		} else
		{
			// get b
			sem_wait(&full_b);
			sem_wait(&mutex_b);
			b = get_b();
			sem_post(&mutex_b);
			sem_post(&empty_b);

			sem_wait(&full_a);
			sem_wait(&mutex_a);
			a = get_a();
			sem_post(&mutex_a);
			sem_post(&empty_a);
		}
		printf("consumer: %d\ta:%d\tb:%d\n", id, a, b);
		sleep(1);
	}
	return NULL;
}

int main(int argc, char **argv)
{
	int i;
	int x[K];
	pthread_t th;
	sem_init(&full_a, 0, 0);
	sem_init(&full_b, 0, 0);
	sem_init(&empty_a, 0, 1);
	sem_init(&empty_b, 0, 1);
	sem_init(&mutex_a, 0, 1);
	sem_init(&mutex_b, 0, 1);
	sem_init(&mutex, 0, 1);
	sem_init(&full, 0, 0);
	for(i = 0; i < N; i++)
	{
		Pthread_create(&th, NULL, producer_a, NULL);
	}
	for(i = 0; i < M; i++)
	{
		Pthread_create(&th, NULL, producer_b, NULL);
	}
	for(i = 0; i < K; i++)
	{
		x[i] = i;
		Pthread_create(&th, NULL, consumer, &x[i]);
	}
	pthread_exit(NULL);
}

