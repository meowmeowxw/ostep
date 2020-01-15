#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
	int x;
	x = 100;
	int rc = fork();
	if(rc < 0)
	{
		fprintf(stderr, "fork failed\n");
		exit(1);
	} else if (rc == 0)
	{
		printf("child process created : %d\n", (int)getpid());
		// This child process will have its own copy of x
		printf("child x : %d\n", x);
		x += 10;
		printf("child x changed : %d\n", x);
	} else
	{
		wait(NULL);
		printf("parent time : pid %d\n", (int)getpid());
		printf("parent x : %d\n", x);
		x += 10;
		printf("parent x changed : %d\n", x);
	}
	return 0;
}

