#define _GNU_SOURCE

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

#define MAX_ARGS 20

int arg_count = 0;
char *path_output = NULL;
char *path_error = NULL;

void print_error(char *msg);
void *Malloc(size_t size);
void parse_line(char *line, char **arg);
void execute_path(char **arg);
void execute_cd(char **arg);
int is_built_int(char *cmd);
void execute_cmd(char **arg);
int check_redirection(char **arg);
void reduce_arg(char **arg);

void print_error(char *msg) {
	fprintf(stderr, "%s: %s\n", msg, strerror(errno));
	exit(1);
}

void *Malloc(size_t size) {
	void *ptr;
	if ((ptr = malloc(size)) == NULL) {
		print_error("malloc");
	}
	return ptr;
}

void parse_line(char *line, char **arg) {
	int count;
	int index = 0;
	arg_count = 0;
	while (*line != '\0' && *line != '\n') {
		count = 0;
		while (*line == ' ' || *line == '\t' || *line == '\n') {
			*line++ = '\0';
		}
		while (*line != '\0' && *line != ' ' && *line != '\t' &&
				*line != '\n') {
			count++;
			line++;
		}
		if (index > MAX_ARGS) {
			puts("shit");
			exit(1);
		}
		arg[index] = Malloc(count);
		line -= count;
		memcpy(arg[index], line, count);
		line += count;
		index++;
	}
	arg_count = index;
}

void execute_cd(char **arg) {
	if (chdir(arg[1]) == -1) {
		print_error("cd");
	}
}

void execute_path(char **arg) { 
	puts("path done"); 
}

int is_built_in(char *cmd) {
	char *built_in[] = {"cd", "exit", "path"};
	for (int i = 0; i < 3; i++) {
		if (strncmp(cmd, built_in[i], strlen(built_in[i])) == 0) {
			return 1;
		}
	}
	return 0;
}

void execute_cmd(char **arg) {
	pid_t pid;
	int status;
	if ((pid = fork()) == -1) {
		print_error("fork");
	}
	if (pid == 0) {
		if(check_redirection(arg) != -1) {
			if(path_error != NULL) {
				int f_error;
				if((f_error = open(path_error, O_CREAT | O_WRONLY)) == -1) {
					print_error("open");
				}
				if(dup2(f_error, 2) == -1) {
					print_error("dup2");
				}
				close(f_error);
			}
			if(path_output != NULL) {
				int f_output;
				if((f_output = open(path_output, O_CREAT | O_WRONLY)) == -1) {
					print_error("open");
				}
				if(dup2(f_output, 1) == -1) {
					print_error("dup2");
				}
				close(f_output);
			}
		}
		reduce_arg(arg);
		if (execvp(arg[0], arg) == -1) {
			print_error("execvp");
		}
		exit(0);
	} else {
		waitpid(pid, &status, 0);
		int exit_status = WEXITSTATUS(status);
		printf("%u  \n", exit_status);
	}
}

void reduce_arg(char **arg)
{
	for(int i = arg_count; i < sizeof(arg); i++) {
		free(arg[i]);
		arg[i] = NULL;
	}
}
void print_prompt()
{
	char *cwd = get_current_dir_name();
	printf("%s\n", cwd);
	free(cwd);
	printf("$ ");
}

int check_redirection(char **arg)
{
	int i, j;
	int quote = 0;
	for(i = 0; i < arg_count; i++)	{
		for(j = 0; j < strlen(arg[i]); j++) {
			if(arg[i][j] == '"') {
				quote = (quote + 1) % 2;
			}
			if(arg[i][j] == '>' && quote == 0) {
				if(arg[i][j-1] == '2') {
					path_error = arg[i + 1];
				} else {
					path_output = arg[i + 1];
				}
				arg_count -= 2;
				return i;
			}
		}
	}
	return -1;
}

void del(char **arg) {
	int i;
	for(i = 0; i < arg_count; i++) {
		free(arg[i]);
		arg[i] = NULL;
	}
	free(arg);
}

int main(int argc, char **argv) {
	char **arg;
	FILE *stream = stdin;
	char *line = NULL;
	ssize_t len = 0;
	ssize_t nread;
	int pid;

	if (argc > 2) {
		fprintf(stderr, "usage: %s [batch-file]\n", argv[0]);
		exit(2);
	}
	if (argc == 2) {
		if ((stream = fopen(argv[1], "r")) == NULL) {
			print_error("fopen");
		}
	}
	while (1) {
		path_output = NULL;
		path_error = NULL;
		print_prompt();
		if ((nread = getline(&line, &len, stream)) != -1) {
			arg = calloc(MAX_ARGS, sizeof(intptr_t));
			parse_line(line, arg);
			if (!is_built_in(arg[0])) {
				execute_cmd(arg);
			} else {
				if (strcmp(arg[0], "cd") == 0) {
					execute_cd(arg);
				}
				if (strcmp(arg[0], "path") == 0) {
					execute_path(arg);
				}
				if (strcmp(arg[0], "exit") == 0) {
					exit(0);
				}
			}
			free(arg);
		} else {
			break;
		}
	}
	free(line); fclose(stream);
	return 0;
}
