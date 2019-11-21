#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main()
{
	int i;
	struct timespec ts;
	for(i=0; i<5; i++) {
		clock_gettime(CLOCK_MONOTONIC, &ts);
		printf("%ld.%ld\n", ts.tv_sec, ts.tv_nsec);
	}
}

