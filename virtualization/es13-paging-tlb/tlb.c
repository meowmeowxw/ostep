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
	int a[NUM] = {0};
	long int res;
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
	int npages = atoi(argv[1]);
	int ntrials = atoi(argv[2]);
	int jump = PAGESIZE / sizeof(int);
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
	res = (BILLION * (end.tv_sec - start.tv_sec) + \
				  (end.tv_nsec - start.tv_nsec)) / ntrials;
	fprintf(stdout, "NPAGES: %d, TIME: %ld\n", npages, res);
	return 0;
}

