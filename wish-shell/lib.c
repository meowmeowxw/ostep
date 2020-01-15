#include "lib.h"

void print_error(char *msg)
{
	fprintf(stderr, "%s: %s\n", msg, strerror(errno));
	exit(1);
}

void *Malloc(size_t size)
{
	void *ptr;
	if((ptr = malloc(size)) == NULL)
	{
		print_error("malloc");
	}
	return ptr;
}

