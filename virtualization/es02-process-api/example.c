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
		execve("/bin/sh", 0, 0);
		printf("this won't print\n");
	} else
	{
		// To executes the child before the parent
		int rc_wait = wait(NULL);
		printf("parent time : pid %d\n", (int)getpid());
	}
	return 0;
}

