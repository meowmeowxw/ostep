#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <mythreads.h>
#include <semaphore.h>

sem_t empty_0, full_0;
sem_t empty_1, full_1;
sem_t mutex_0, mutex_1;

int hammer[2];
int hammer_pitched_0 = 0;
int hammer_pitched_1 = 0;
int hammer_pitched = 0;

void *take_stake(void *arg)
{
	while(1)
	{
		sem_wait(&empty_0);
		sem_wait(&mutex_0);
		hammer[0] = 0;
		sem_post(&mutex_0);

		sem_wait(&empty_1);
		sem_wait(&mutex_1);
		hammer[1] = 0;
		sem_post(&mutex_1);

		puts("hammer 0 ready");
		puts("hammer 1 ready\n");

		sem_post(&full_0);
		sem_post(&full_1);
		sleep(2);
		hammer_pitched++;
		if(hammer_pitched == 5)
		{
			puts("take stake pitched 5*2 stakes, going to sleep");
			sleep(3);
		}
	}
	return NULL;
}

void *hammer_do_0(void *arg)
{
	while(1)
	{
		
		sem_wait(&full_0);
		sem_wait(&mutex_0);
		puts("hammer 0 start");
		hammer[0] = 1;
		hammer_pitched_0++;
		if(hammer_pitched_0 == 4)
		{
			puts("hammer 0 pitched 4 stakes, going to sleep");
			sleep(5);
			hammer_pitched_0 = 0;
		}
		sleep(2);
		puts("hammer 0 finish");
		sem_post(&mutex_0);
		sem_post(&empty_0);
	}
	return NULL;
}

void *hammer_do_1(void *arg)
{
	while(1)
	{
		sem_wait(&full_1);
		sem_wait(&mutex_1);
		puts("hammer 1 start");
		hammer[1] = 1;
		hammer_pitched_1++;
		if(hammer_pitched_1 == 4)
		{
			puts("hammer 1 pitched 4 stakes, going to sleep");
			sleep(7);
			hammer_pitched_1 = 0;
		}
		sleep(2);
		puts("hammer 1 finish");	
		sem_post(&mutex_1);
		sem_post(&empty_1);
	}
	return NULL;
}

int main(int argc, char **argv)
{
	pthread_t th;
	setbuf(stdout, NULL);
	sem_init(&empty_0, 0, 1);
	sem_init(&empty_1, 0, 1);
	sem_init(&full_0, 0, 0);
	sem_init(&full_0, 0, 0);
	sem_init(&mutex_0, 0, 1);
	sem_init(&mutex_1, 0, 1);
	Pthread_create(&th, NULL, hammer_do_0, NULL);
	Pthread_create(&th, NULL, hammer_do_1, NULL);
	Pthread_create(&th, NULL, take_stake, NULL);
	pthread_exit(NULL);
}

