#include <stdio.h>
#include <stdlib.h>
#include "mythreads.h"

volatile int done = 0;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t c = PTHREAD_COND_INITIALIZER;

void thr_exit()
{
	Pthread_mutex_lock(&m);
	done = 1;
	Pthread_cond_signal(&c);
	Pthread_mutex_unlock(&m);
}

void thr_join()
{
	Pthread_mutex_lock(&m);
	while(done == 0)
	{
		Pthread_cond_wait(&c, &m);
	}
	Pthread_mutex_unlock(&m);
}

void *child(void *arg)
{
	puts("child");
	thr_exit();
	return NULL;
}

int main(int argc, char **argv)
{
	pthread_t c;
	puts("parent: begin");
	Pthread_create(&c, NULL, child, NULL);
	thr_join();
	puts("parent: end");
	return 0;
}

