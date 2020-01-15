#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define BSIZE 100

int main(int argc, char **argv) {
    int fildes[2];
    char buf[BSIZE];
    int status_pipe;
    ssize_t nbytes;
    status_pipe = pipe(fildes);
    if (status_pipe == -1) {
        fprintf(stderr, "error in pipe\n");
        exit(1);
    }
    switch (fork()) {
    case -1:
        fprintf(stderr, "error in fork\n");
        exit(1);
    case 0:
        close(fildes[1]);                     /* Write end is unused */
        nbytes = read(fildes[0], buf, BSIZE); /* Get data from pipe */
        if (nbytes == -1) {
            fprintf(stderr, "error in read\n");
            exit(1);
        }
        printf("read : %s", buf);
        /* At this point, a further read would see end of file ... */
        close(fildes[0]); /* Finished with pipe */
        exit(EXIT_SUCCESS);
    default:                                   /* Parent - writes to pipe */
        close(fildes[0]);                      /* Read end is unused */
        write(fildes[1], "Hello world\n", 12); /* Write data on pipe */
        close(fildes[1]);                      /* Child will see EOF */
        exit(EXIT_SUCCESS);
    }
    return 0;
}
