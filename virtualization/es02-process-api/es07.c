#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
	int fork_rc;
	int wait_rc;
	fork_rc = fork();
	if (fork_rc < 0)
	{
		fprintf(stderr, "couldn't fork\n");
	} else if (fork_rc == 0)
	{
		printf("child time : %d\n", (int)getpid());
		// close(1);
		close(STDOUT_FILENO);
		puts("YOLO");
	} else 
	{
		wait_rc = wait(NULL);
		printf("parent time : %d\n", (int)getpid());
		if (wait_rc == -1)
		{
			printf("errno : %s\n", strerror(errno));
		}

	}
	return 0;
}

