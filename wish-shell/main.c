#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>
#include "lib.h"

u_int8_t batch_mode = 0;

int main(int argc, char **argv)
{
	char prompt[] = "prompt> ";
	FILE *stream = stdin; 
	char *line = NULL;
	ssize_t len = 0;
	ssize_t nread;
	int pid;

	if(argc > 2)
	{
		fprintf(stderr, "usage: %s [batch-file]\n", argv[0]);
		exit(2);
	}
	if(argc == 2)
	{
		batch_mode = 1;
		if((stream = fopen(argv[1], "r")) == NULL)
			print_error("fopen");
	}
	while(1)
	{
		if((nread = getline(&line, &len, stream)) != -1)
		{
			if((pid = fork()) == -1)
				print_error("fork");
			if(pid == 0)
			{
				puts("child");
				exit(0);
			} else
			{
				wait(NULL);
				puts("parent");
			}
		}
	}
	free(line);
	fclose(stream);
	return 0;
}

