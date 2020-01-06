#include <unistd.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <stdint.h>
#include <inttypes.h>
#include <pthread.h> 
#include "mythreads.h"

#define SYNC_MAX 10

pthread_mutex_t  sync_lock; 
pthread_cond_t   sync_cond; 
int  sync_count = 0; 

void SyncPoint(void) 
{ 
	int index;
	/* blocca l'accesso al counter */ 
	Pthread_mutex_lock(&sync_lock); 

	index = sync_count;
	/* incrementa il counter di quelli arrivati*/ 
	sync_count++; 

	/* controlla se deve aspettare o no */ 
	if(sync_count < SYNC_MAX) 
	{
		Pthread_cond_wait(&sync_cond, &sync_lock); 
		/* senza questa signal ne terminano solo 2 */
		Pthread_cond_signal (&sync_cond); 
	} else
	{
		sync_count = 0;
		/* tutti hanno raggiunto il punto di barriera */ 
		pthread_cond_broadcast (&sync_cond); 
	}
	while(sync_count != index)
	{
		Pthread_cond_wait(&sync_cond, &sync_lock);
	}
	sync_count++;
	pthread_cond_broadcast(&sync_cond);
	/* sblocca il mutex */ 
	Pthread_mutex_unlock (&sync_lock); 
} 

void *Thread (void *arg) 
{ 
	pthread_t  th; 
	th = pthread_self(); 
	printf ("%lu\n", th); 
	SyncPoint(); 
	printf("Sono %lu e sono uscito \n", th); 
	return NULL;
} 

int main () 
{ 
	pthread_t th[SYNC_MAX]; 
	intptr_t i;

	Pthread_cond_init(&sync_cond, NULL);
	Pthread_mutex_init(&sync_lock, NULL); 

	for(i=0;i<SYNC_MAX;i++) 
	{
		Pthread_create(&th[i], NULL, Thread, NULL); 
	}
	for(i=0;i<SYNC_MAX;i++) 
	{
		Pthread_join(th[i], NULL);
	}
	pthread_mutex_destroy(&sync_lock); 
	pthread_cond_destroy(&sync_cond); 
	return 0;
} 
  
