#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <semaphore.h>
#include <pthread.h>
#include <mythreads.h>
#include <sys/syscall.h>

#define NUM_CUSTOMERS 10

int ticket;
int display;
pthread_mutex_t lock_ticket, lock_display;
sem_t s[NUM_CUSTOMERS];

void shelf(int id)
{
	printf("serving: %d\n", id);
	Pthread_mutex_lock(&lock_display);
	display += 1;
	Pthread_mutex_unlock(&lock_display);
	sleep(1);
	return;	
}

void *customer_do(void *arg)
{
	int myticket;
	Pthread_mutex_lock(&lock_ticket);
	myticket = ticket;
	ticket++;
	Pthread_mutex_unlock(&lock_ticket);
	printf("customer %ld has ticket: %d\n", \
			syscall(SYS_gettid) % NUM_CUSTOMERS, myticket);
	fflush(stdout);
	sem_wait(&s[myticket]);
	shelf(myticket);
	sem_post(&s[myticket + 1]);
	sem_post(&s[myticket + 2]);
	return NULL;
}

int main(int argc, char **argv)
{
	int i;
	pthread_t customer[NUM_CUSTOMERS];
	ticket = 0;
	display = 0;
	Pthread_mutex_init(&lock_ticket, NULL);
	Pthread_mutex_init(&lock_display, NULL);
	sem_init(&s[0], 0, 1);
	sem_init(&s[1], 0, 1);
	for(i = 2; i < NUM_CUSTOMERS; i++)
	{
		sem_init(&s[i], 0, 0);
	}
	for(i = 0; i < NUM_CUSTOMERS; i++)
	{
		Pthread_create(&customer[i], NULL, customer_do, NULL);
	}
	for(i = 0; i < NUM_CUSTOMERS; i++)
	{
		Pthread_join(customer[i], NULL);
	}
	return 0;
}

