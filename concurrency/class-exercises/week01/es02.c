#include <errno.h>
#include <inttypes.h> /* PRIiPTR */
#include <pthread.h>
#include <stdint.h> /* uintptr_t */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define NUM_THREADS 5

void *printhello(void *arg) {
    printf("received: %d\n", *(int *)arg);
    pthread_exit(NULL);
    free(arg);
}

int main() {
    int i;
    int rc;
    int *x;
    pthread_t tid;
    char error[256];
    for (i = 1;; i++) {
        x = malloc(sizeof(int));
        *x = i;
        rc = pthread_create(&tid, NULL, printhello, x);
        if (rc) {
            strerror_r(rc, error, sizeof(error));
            printf("error: %s\n", error);
            exit(1);
        }
    }
    pthread_exit(NULL);
    return (0);
}
