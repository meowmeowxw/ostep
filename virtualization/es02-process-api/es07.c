#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
		close(STDOUT_FILENO);
		printf("try\n");
	} else 
	{
		wait_rc = wait(NULL);
		printf("parent time : %d\n", (int)getpid());
	}
	return 0;
}
