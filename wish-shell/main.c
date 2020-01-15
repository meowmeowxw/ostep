#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>

#define MAX_ARGS 20

void print_error(char *msg)
{
	fprintf(stderr, "%s: %s\n", msg, strerror(errno));
	exit(1);
}

void *Malloc(size_t size)
{
	void *ptr;
	if((ptr = malloc(size)) == NULL)
		print_error("malloc");
	return ptr;
}

void **parse_line(char *line, char **arg)
{
	int count;
	int index = 0;
	while(*line != '\0' && *line != '\n')
	{
		count = 0;
		while (*line == ' ' || *line == '\t' || *line == '\n')
			*line++ = '\0';
		while (*line != '\0' && *line != ' ' && *line != '\t' && *line != '\n') 
		{
		    count++;
			line++;
		}
		if(index > MAX_ARGS)
		{
			puts("shit");
			exit(1);
		}
		arg[index] = Malloc(count);
		line -= count;
		memcpy(arg[index], line, count);
		line += count;
		index++;
	}
}

void execute_cd(char **arg)
{
	if(chdir(arg[1]) == -1)
		print_error("cd");
}

void execute_path(char **arg)
{
	puts("path done");
}

int is_built_in(char *cmd)
{
	char *built_in[] = {"cd", "exit", "path"};
	for(int i = 0; i < 3; i++)
	{
		if(strncmp(cmd, built_in[i], strlen(built_in[i])) == 0)
			return 1;
	}
	return 0;
}

void execute_cmd(char **arg)
{
	pid_t pid;
	int status;

	if((pid = fork()) == -1)
		print_error("fork");
	if(pid == 0)
	{
		if(execvp(arg[0], arg) == -1)
			print_error("execvp");
		exit(0);
	} else
	{
		waitpid(pid, &status, 0);
		int exit_status = WEXITSTATUS(status);
		printf("%u  \n", exit_status);
	}
}

int main(int argc, char **argv)
{
	char prompt[] = "prompt> ";
	FILE *stream = stdin; 
	char *line = NULL;
	ssize_t len = 0;
	ssize_t nread;
	int pid;
	char **arg;
	if(argc > 2)
	{
		fprintf(stderr, "usage: %s [batch-file]\n", argv[0]);
		exit(2);
	}
	if(argc == 2)
	{
		if((stream = fopen(argv[1], "r")) == NULL)
			print_error("fopen");
	}
	while(1)
	{
		printf("%s", prompt);
		if((nread = getline(&line, &len, stream)) != -1)
		{
			arg = calloc(MAX_ARGS, 8);
			parse_line(line, arg);
			if(!is_built_in(arg[0]))
				execute_cmd(arg);
			else
			{
				if(strcmp(arg[0], "cd") == 0)
					execute_cd(arg);
				if(strcmp(arg[0], "path") == 0)
					execute_path(arg);
				if(strcmp(arg[0], "exit") == 0)
					exit(0);
			}
			free(arg);
		} else
			break;
	}
	free(line);
	fclose(stream);
	return 0;
}

