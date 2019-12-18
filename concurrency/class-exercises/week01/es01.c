#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mythreads.h"

#define NUM 10

void *thread(void *arg)
{
	sleep(2);
	printf("received: %d\n", *(int *)arg);
	free(arg);
	return NULL;
}
int main(int argc, char **argv)
{
	int i;
	int *x;
	pthread_t pt[NUM];
	srand(time(0));
	for(i = 0; i < NUM; ++i)
	{
		x = malloc(sizeof(int));
		*x = rand();
		Pthread_create(&pt[i], NULL, thread, x);
	}
	for(i = 0; i < NUM; ++i)
	{
		Pthread_join(pt[i], NULL); 
	}
	return 0;
}

