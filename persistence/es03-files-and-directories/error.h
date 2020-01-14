void print_error()
{
	fprintf(stderr, "%s\n", strerror(errno));
	exit(1);
}

