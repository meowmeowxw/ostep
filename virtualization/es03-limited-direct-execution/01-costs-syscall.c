#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BILLION 1000000000L

void cost()
{
	char buf[1];
	int fd;
	time_t seconds;
	long int nanoseconds;
	// defined in /usr/include/bits/types/struct_timespec.h
	struct timespec start, end;
	fd = open("./01-test.txt", O_RDONLY);
	if(clock_gettime(CLOCK_REALTIME, &start) == -1)
	{
		fprintf(stderr, "error in clock_getres\n");
		exit(1);
	}
	read(fd, buf, sizeof(buf));
	if(clock_gettime(CLOCK_REALTIME, &end) == -1)
	{
		fprintf(stderr, "error in clock_getres\n");
		exit(1);
	}
	printf("%ld.%ld\n", start.tv_sec, start.tv_nsec);
	printf("%ld.%ld\n", end.tv_sec, end.tv_nsec);
	seconds = end.tv_sec - start.tv_sec;
	nanoseconds = end.tv_nsec - start.tv_nsec;
	printf("%ld\n", seconds);
	printf("%ld\n", nanoseconds);
}
int main(int argc, char **argv)
{
	cost();
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


