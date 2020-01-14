#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <errno.h>
#include <pwd.h>
#include <getopt.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "error.h"

#define MAX_ARGS 3
#define MAX_LENGTH_NAME_FILE 1024

int extended = 0;

void print_file(struct stat *info)
{
	struct passwd *p;
	p = getpwuid(info->st_uid);
	switch(info->st_mode & S_IFMT)
	{
		case S_IFBLK:  printf("b"); break;
		case S_IFCHR:  printf("c"); break;
		case S_IFDIR:  printf("d"); break;
		case S_IFIFO:  printf("f"); break;
		case S_IFLNK:  printf("l"); break;
		case S_IFREG:  printf("-"); break;
		case S_IFSOCK: printf("s"); break;
		default:       printf("?"); break;
	}
	printf((info->st_mode & S_IRUSR) ? "r" : "-");
	printf((info->st_mode & S_IWUSR) ? "w" : "-");
	printf((info->st_mode & S_IXUSR) ? "x" : "-");
	printf((info->st_mode & S_IRGRP) ? "r" : "-");
	printf((info->st_mode & S_IWGRP) ? "w" : "-");
	printf((info->st_mode & S_IXGRP) ? "x" : "-");
	printf((info->st_mode & S_IROTH) ? "r" : "-");
	printf((info->st_mode & S_IWOTH) ? "w" : "-");
	printf((info->st_mode & S_IXOTH) ? "x" : "-");
	printf("\t%s\t%lu\t", p->pw_name, info->st_size);
	return;
}

int main(int argc, char **argv)
{
	struct dirent *d;
	struct stat info;
	DIR *dir;
	char *path = ".";
	char file_path[MAX_LENGTH_NAME_FILE];
	int option_index;
	opterr = 0;

	while((option_index = getopt(argc, argv, "l:")) != -1)
	{
		switch(option_index)
		{
			case 'l':
				path = optarg;
				extended = 1;
				break;
			case '?':
				if(optopt == 'l')
					extended = 1;
				break;
			default:
				break;
		}
		
	}
	if(!extended && argc > 1)
		path = argv[1];

	if((dir = opendir(path)) == NULL)
		print_error();
	
	if(stat(path, &info) == -1)
		print_error();

	if(S_ISDIR(info.st_mode))
	{
		while((d = readdir(dir)) != NULL)
		{
			if(extended)
			{
				strncpy(file_path, path, strlen(path));
				strcat(file_path, "/");
				strncat(file_path, d->d_name, strlen(d->d_name));
				if(stat(file_path, &info) == -1)
				{
					print_error();
				}
				print_file(&info);
				memset(file_path, 0, MAX_LENGTH_NAME_FILE);
			}
			printf("%s\n", d->d_name);
		}
		closedir(dir);
	} else
	{
		if(extended)
			print_file(&info);
		printf("%s\n", path);
	}
	return 0;
}

