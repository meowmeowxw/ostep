#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <sys/stat.h>

void print_stat_info(struct stat *info)
{
	printf("Size: %ld\tBlocks: %ld\tIO Block: %ld\t", \
			info->st_size, info->st_blocks, info->st_blksize);
	switch(info->st_mode & S_IFMT)
	{
		case S_IFBLK:  printf("block device\n");      break;
		case S_IFCHR:  printf("character device\n");  break;
		case S_IFDIR:  printf("directory\n");         break;
		case S_IFIFO:  printf("FIFO/pipe\n");         break;
		case S_IFLNK:  printf("symlink\n");           break;
		case S_IFREG:  printf("regular file\n");      break;
		case S_IFSOCK: printf("socket\n");            break;
		default:       printf("unknown?\n");          break;
	}
	printf("Device: %ld\tInode: %ld\tLinks:%ld\n", \
			info->st_dev, info->st_ino, info->st_nlink);
}

int main(int argc, char **argv)
{
	if(argc == 1)
	{
		fprintf(stderr, "usage: ./es01-stat.exe <file>\n");
		exit(1);
	}
	int rc;
	int i;
	struct stat info;
	for(i = 1; i < argc; i++)
	{
		rc = stat(argv[1], &info);
		assert(rc == 0);
		printf("File: %s\n", argv[1]);
		print_stat_info(&info);
	}
	return 0;
}

