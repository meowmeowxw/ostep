#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define LENGTH_BYTES 3
#define STATS 100

struct timespec cost(unsigned int nbytes)
{
	char buf[1];
	int fd;
	// defined in /usr/include/bits/types/struct_timespec.h
	struct timespec start, end, res;
	if(clock_gettime(CLOCK_REALTIME, &start) == -1)
	{
		fprintf(stderr, "error in clock_getres\n");
		exit(1);
	}
	for(int i = 0; i < nbytes; i++)
	{
		read(STDIN_FILENO, buf, 0) ;
	}
	if(clock_gettime(CLOCK_REALTIME, &end) == -1)
	{
		fprintf(stderr, "error in clock_getres\n");
		exit(1);
	}
	res.tv_sec = (end.tv_sec - start.tv_sec) / nbytes;
	res.tv_nsec = (end.tv_nsec - start.tv_nsec) / nbytes;
	return res;
}
void statistics()
{
	int nbytes[] = {100, 1000, 10000};
	double average_nsec[] = {0, 0, 0};
	struct timespec ts[STATS];
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < STATS; j++)
		{
			ts[j] = cost(nbytes[i]);
			average_nsec[i] += ts[j].tv_nsec;
		}
		printf("average for read %d times : %ld ns\n", \
				nbytes[i], (double)(average_nsec[i] / STATS));
	}
	return;
}
int main(int argc, char **argv)
{
	statistics();
	return 0;
}

/*
void syscall_time()
{
	int a = 10;
	int b;
	__asm__ (
			"rdtsc;"
			"xor %%eax, %%eax;"
			"movl %1, %%eax;"
			"movl %%eax, %0;"
			:"=r" (b)
			:"r" (a)
			:"%eax", "%ecx"
			);
	printf("%d\n", b);
}
*/


