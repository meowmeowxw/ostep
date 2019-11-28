#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>


#define NUM 1000000
#define PAGESIZE 4
#define BILLION 1000000000L

int main(int argc, char **argv)
{
	int i, j;
	int a[NUM] = {0};
	long int res;
	struct timespec start, end;
	if(argc != 3)
	{
		puts("usage: ./tlb npages ntrials");
		exit(1);
	}
	int npages = atoi(argv[1]);
	int ntrials = atoi(argv[2]);
	int jump = PAGESIZE / sizeof(int);
	if(clock_gettime(CLOCK_REALTIME, &start) == -1)
	{
		fprintf(stderr, "error in clock_getres\n");
		exit(2);
	}
	for(j = 0; j < ntrials; j++)
	{
		for(i = 0; i < (npages * jump); i += jump)
		{
			a[i] += 1;
		}
	}
	if(clock_gettime(CLOCK_REALTIME, &end) == -1)
	{
		fprintf(stderr, "error in clock_getres\n");
		exit(2);
	}
	res = (BILLION * (end.tv_sec - start.tv_sec) + \
				  (end.tv_nsec - start.tv_nsec)) / ntrials;
	printf("result: %ld\n", res);
	
	return 0;
}

