#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

void print_error() {
    fprintf(stderr, "%s\n", strerror(errno));
    exit(1);
}
