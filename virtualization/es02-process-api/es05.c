#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

int main(int argc, char **argv)
{
	int fork_rc = fork();
	if(fork_rc < 0)
	{
		fprintf(stderr, "fork failed\n");
		exit(1);
	} else if (fork_rc == 0)
	{
		printf("child time : %d\n", (int)getpid());	
		// Nothing happen if we wait in the child
		pid_t wait_rc = wait(NULL);
		int wait_error = errno;
		printf("errno : %s\n", strerror(wait_error));
		printf("wait_rc : %d\n", wait_rc);
	} else
	{
		printf("parent time : %d\n", (int)getpid());
		pid_t wait_rc = wait(NULL);
		int wait_error = errno;
		printf("errno : %s\n", strerror(wait_error));
		printf("wait_rc is the pid of the child : %d\n", wait_rc);
	}
	return 0;
}

