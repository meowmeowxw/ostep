#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <mythreads.h>

pthread_cond_t sync_0;
pthread_cond_t sync_1;
pthread_cond_t wait;
pthread_mutex_t lock;

int hammer[2] = {1, 1};

void *take_stake(void *arg)
{
	while(1)
	{
		Pthread_mutex_lock(&lock);
		while(hammer[0] == 0 && hammer[1] == 0)
		{
			Pthread_cond_wait(&wait, &lock);
		}
		hammer[0] = 0;
		hammer[1] = 0;
		puts("\nhammer 0/1 ready\n");
		Pthread_cond_signal(&sync_0);
		Pthread_cond_signal(&sync_1);
		Pthread_mutex_unlock(&lock);
	}
	return NULL;
}

void *hammer_do_0(void *arg)
{
	while(1)
	{
		Pthread_mutex_lock(&lock);
		while(hammer[0] == 1)
		{
			Pthread_cond_wait(&sync_0, &lock);
		}
		puts("hammer 0 start");
		hammer[0] = 1;
		Pthread_mutex_unlock(&lock);
		sleep(1);
		Pthread_mutex_lock(&lock);
		puts("hammer 0 finish");
		Pthread_cond_signal(&wait);
		Pthread_mutex_unlock(&lock);
	}
	return NULL;
}

void *hammer_do_1(void *arg)
{
	while(1)
	{
		Pthread_mutex_lock(&lock);
		while(hammer[1] == 1)
		{
			Pthread_cond_wait(&sync_1, &lock);
		}
		puts("hammer 1 start");
		hammer[1] = 1;
		Pthread_mutex_unlock(&lock);
		sleep(1);
		puts("hammer 1 finish");
		Pthread_cond_signal(&wait);
	}
	return NULL;
}

int main(int argc, char **argv)
{
	pthread_t th;
	setbuf(stdout, NULL);
	Pthread_mutex_init(&lock, NULL);
	Pthread_cond_init(&wait, NULL);
	Pthread_cond_init(&sync_0, NULL);
	Pthread_cond_init(&sync_1, NULL);
	Pthread_create(&th, NULL, hammer_do_0, NULL);
	Pthread_create(&th, NULL, hammer_do_1, NULL);
	Pthread_create(&th, NULL, take_stake, NULL);
	pthread_exit(NULL);
}

