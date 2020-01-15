#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char **argv) {
    int rc = fork();
    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        printf("child time : %d\n", (int)getpid());
    } else {
        // wait for any child process (-1)
        // store information status in NULL
        // option 0
        pid_t waitpid_rc = waitpid(-1, NULL, 0);
        printf("wait_rc is the pid of the child : %d\n", waitpid_rc);
        printf("parent time : %d\n", (int)getpid());
    }
    return 0;
}
