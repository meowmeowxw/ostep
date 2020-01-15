#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char **argv) {
    char buf[20];
    int fd = open("./test02.txt", O_WRONLY);
    // It will write sequencially on the file even if I close the file
    // descriptor both in the parent and in the children.
    int rc = fork();
    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        printf("child process created : %d\n", (int)getpid());
        strcpy(buf, "aaaa");
        write(fd, buf, 4);
        close(fd);
    } else {
        // int rc_wait = wait(NULL);
        printf("parent time : pid %d\n", (int)getpid());
        strcpy(buf, "bbbb");
        write(fd, buf, 4);
        close(fd);
    }
    return 0;
}
