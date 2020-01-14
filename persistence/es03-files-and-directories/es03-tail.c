#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "error.h"
// stat
// lseek
// open
// read
// close

int main(int argc, char **argv)
{
	struct stat info;
	char *path;
	char *buf;
	char *final;
	char chr[0];
	int option_index;
	int fd;
	long int nlines = 0;
	long unsigned int total_size = 1;
	long unsigned int i = 0;
	int length_line = 0;
	opterr = 0;
	
	if(argc != 4)
	{
		fprintf(stderr, "Usage: %s -n lines filename\n", argv[0]);
		exit(1);
	}

	while((option_index = getopt(argc, argv, "n:")) != -1)
	{
		switch(option_index)
		{
			case 'n':
				nlines = atoll(optarg);
				break;
			default:
				fprintf(stderr, "Usage: %s -n lines filename\n", argv[0]);
				exit(1);
		}
	}
	path = argv[optind];
	printf("nlines: %lu\n", nlines);

	if(stat(path, &info) == -1)
		print_error();
	
	if((fd = open(path, O_RDONLY)) == -1)
		print_error();
	
	if(lseek(fd, -1, SEEK_END) == -1)
		print_error();
	
	while(nlines >= 0)
	{
		if(read(fd, chr, 1) == -1)
			print_error();

		length_line++;
		total_size++;
		if(chr[0] == '\n')
		{
			if(lseek(fd, -(length_line + 1), SEEK_CUR) == -1)
			{
				fprintf(stderr, "porcodioseek\n");
			}
			length_line = 0;
			nlines--;
		} else
		{
			if(lseek(fd, -2, SEEK_CUR) == -1)
			{
				break;
			}
		}
	}
	printf("total_size: %lu\n", total_size);
	buf = malloc(total_size + 1);
	lseek(fd, -(total_size + 1), SEEK_END);
	if(read(fd, buf, (total_size + 1)) == -1)
		print_error();
	printf("%s\n", buf);
	return 0;
}

