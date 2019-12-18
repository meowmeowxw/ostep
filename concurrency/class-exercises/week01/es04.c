#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include "mythreads.h"

#define NUM_THREADS 4

typedef struct __bho
{
	char str[100];
	int n;		// pthread to create
	int index;
}bho;

void *thread(void *arg)
{
	usleep(1000);
	int m;
	int i;
	bho *received, *send;
	received = (bho *)arg;
	printf("thread: %d\t index: %d\n", received->n, received->index);
	m = received->n;	
	if(m)
	{
		pthread_t pt[NUM_THREADS];
		for(i = 0; i < m - 1; i++)
		{
			send = malloc(sizeof(bho));
			send->n = m - 1;
			send->index = i;
			strcpy(send->str, "random");
			Pthread_create(&pt[i], NULL, thread, send);
		}
		for(i = 0; i < m - 1; i++)
		{
			Pthread_join(pt[i], (void **)&send);
			printf("received %s\n", send->str);
			free(send);
		}
	}
	sprintf(received->str, "%d %d", received->n, received->index);
	pthread_exit(received);
}

int main(int argc, char **argv)
{
	pthread_t pt[NUM_THREADS];
	bho *send;
	int i;
	srand(time(0));
	for(i = 0; i < NUM_THREADS; i++)
	{	
		send = malloc(sizeof(bho));
		send->n = NUM_THREADS - 1;
		send->index = i;
		strcpy(send->str, "random");
		printf("creating thread: %d\n", i);
		Pthread_create(&pt[i], NULL, thread, send);	
	}
	for(i = 0; i < NUM_THREADS; i++)
	{
		Pthread_join(pt[i], (void **)&send);
		printf("main received %s\n", send->str);
		free(send);
	}
	return 0;
}

