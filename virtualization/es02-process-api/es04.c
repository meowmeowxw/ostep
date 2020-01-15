#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

char LS_PATH[] = "/bin/ls";

void call_execl()
{
	execl(LS_PATH, "ls");
}

void call_execvp()
{
	char *arguments[] = {LS_PATH, "-l"};
	execvp(LS_PATH, arguments);
}

int main(int argc, char **argv)
{
	int rc = fork();
	if(rc < 0)
	{
		fprintf(stderr, "fork failed\n");
		exit(1);
	} else if (rc == 0)
	{
		puts("child time");	
		// call_execvp();
		call_execl();
	} else
	{
		wait(NULL);
		puts("parent time");
	}
	return 0;
}

