/* 
*	NOTE: In order to be effective, a feature test macro  must  be  defined
*   before including any header files.
*/
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sched.h>

#define NUM 1000000
#define PAGESIZE 4096 // get_conf PAGE_SIZE
#define BILLION 1000000000L
#define CPU 0

int main(int argc, char **argv)
{
	int i, j;
	int *a;
	long int total_time;
	cpu_set_t set;
	struct timespec start, end;
	/*
	 * initial check number of arguments
	 */
	if(argc != 3)
	{
		puts("usage: ./tlb npages ntrials");
		exit(1);
	}
	/*	
	 *	set affinity to cpu, the process will run on the cpu 0
	 */
	CPU_SET(0, &set);
	if(sched_setaffinity(getpid(), sizeof(set), &set) == -1)
	{
		fprintf(stderr, "couldn't pin the process on a particular cpu\n");
		exit(2);
	}
	long int npages = atoi(argv[1]);
	int ntrials = atoi(argv[2]);
	int jump = PAGESIZE / sizeof(int);
	a = calloc((npages * jump), sizeof(int));
	if(a == NULL)
	{
		fprintf(stderr, "malloc error\n");
		exit(1);
	}
	/* 
	 * access tlb first
	 */
	for(i = 0; i < (npages * jump); i += jump)
	{
		a[i] += 1;
	}
	/*	
	 *	start the timer
	 */
	if(clock_gettime(CLOCK_REALTIME, &start) == -1)
	{
		fprintf(stderr, "error in clock_getres\n");
		exit(3);
	}
	/*
	 * iterate the loop npages time
	 */
	for(j = 0; j < ntrials; j++)
	{
		for(i = 0; i < (npages * jump); i += jump)
		{
			a[i] += 1;
		}
	}
	/*	
	 * stop the timer and compute the result
	 */
	if(clock_gettime(CLOCK_REALTIME, &end) == -1)
	{
		fprintf(stderr, "error in clock_getres\n");
		exit(3);
	}
	total_time = (BILLION * (end.tv_sec - start.tv_sec) + \
				  (end.tv_nsec - start.tv_nsec)) / ntrials;
	
	fprintf(stdout, "%ld,%g\n", npages, (double)total_time/npages);
	//fprintf(stdout, "%ld,%ld\n", npages, total_time);
	//fprintf(stdout, "NPAGES: %ld, TIME: %ld, AVERAGE: %g\n",
	//		npages, total_time, (double)total_time/npages);
	return 0;
}

