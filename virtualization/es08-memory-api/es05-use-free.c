#include <stdio.h>
#include <stdlib.h>

void func(int x)
{
	;
}

int main(int argc, char **argv)
{
	void *x = &func;
	free(x);
	return 0;
}

/*
 * munmap_chunk(): invalid pointer
 * [1]    20832 abort (core dumped)  ./es05-use-free
 */

