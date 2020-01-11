#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <errno.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
#define MAX_ARGS 3
int extended = 0;
int main(int argc, char **argv)
{
	struct dirent *d;
	struct passwd *p;
	DIR *dir;
	char *path = ".";
	char *file;
	char option[3] = "-l\00";
	struct stat info;
	if(argc >= 2)
	{
		if(strncmp(argv[1], option, strlen(option)) == 0)
		{
			extended = 1;
			if(argc == 3)
			{
				path = argv[2];
			}
		} else
		{
			path = argv[1];
		}
	}
	puts(path);
	dir = opendir(path);
	if(dir == NULL)
	{
		fprintf(stderr, "%s\n", strerror(errno));
		exit(1);
	}
	while((d = readdir(dir)) != NULL)
	{
		if(extended)
		{
			file = d->d_name;
			if(stat(file, &info) == -1)
			{
				continue;
			}
			p = getpwuid(info.st_uid);
			printf("%s\t%lu\t", p->pw_name, info.st_size);
		}
		printf("%s\n", d->d_name);
	}
	closedir(dir);
	return 0;
}
