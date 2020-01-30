#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <limits.h> /* for OPEN_MAX */
#include <mythreads.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>   /* per strerror_r  and  memset */
#include <sys/mman.h> /* shm_* stuff, and mmap() */
#include <sys/stat.h>
#include <sys/time.h> /* timeval{} for select() */
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>   /* timespec{} for pselect() */
#include <unistd.h> /* exit() etc */

#define NUM_CAVEMEN 3
#define A 0
#define B 1

typedef struct __shared_buffer {
    pthread_mutex_t lock;
    pthread_cond_t full, turn;
    int is_full;
    int queue;
} shared_buffer;

shared_buffer *shr_buff;

void print_status_dinosaur() {
    char q = shr_buff->queue == A ? 'A' : 'B';
    printf("\ncavemen: %d\ton queue: %c\n", shr_buff->is_full, q);
    puts("move queue to the other side\n");
    fflush(stdout);
}

void print_status_caveman(int id, int myqueue) {
    char q = myqueue == A ? 'A' : 'B';
    printf("caveman: %d\tis waiting on: %c\n", id, q);
    fflush(stdout);
}

void *dinosaur(void) {
    while (1) {
        Pthread_mutex_lock(&shr_buff->lock);
        while (shr_buff->is_full != 2) {
            Pthread_cond_wait(&shr_buff->full, &shr_buff->lock);
        }
        print_status_dinosaur();
        shr_buff->is_full = 0;
        shr_buff->queue = (shr_buff->queue + 1) % 2;
        Pthread_cond_broadcast(&shr_buff->turn);
        Pthread_mutex_unlock(&shr_buff->lock);
        sleep(1);
    }
    return NULL;
}

void *caveman(int id) {
    int myqueue = id % 2;
    while (1) {
        Pthread_mutex_lock(&shr_buff->lock);
        print_status_caveman(id, myqueue);
        while (myqueue != shr_buff->queue || shr_buff->is_full == 2) {
            Pthread_cond_wait(&shr_buff->turn, &shr_buff->lock);
        }
        shr_buff->is_full++;
        printf("caveman: %d\tget on\n", id);
        myqueue = (myqueue + 1) % 2;
        Pthread_cond_signal(&shr_buff->full);
        Pthread_mutex_unlock(&shr_buff->lock);
        sleep(2);
    }
    return NULL;
}

int main(int argc, char **argv) {
    // Without error checks :/
    int shmfd, rc;
    pid_t pid;
    int shared_seg_size = sizeof(shared_buffer);
    pthread_mutexattr_t mattr;
    pthread_condattr_t cvattr;

    shmfd = shm_open("/pedala", O_CREAT | O_RDWR, S_IRWXU);
    assert(shmfd >= 0);
    rc = ftruncate(shmfd, shared_seg_size);
    assert(rc == 0);

    shr_buff = (shared_buffer *)mmap(
        NULL, shared_seg_size, PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);

    shr_buff->queue = A;
    shr_buff->is_full = 0;
    pthread_mutexattr_init(&mattr);
    pthread_condattr_init(&cvattr);
    pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_SHARED);
    pthread_condattr_setpshared(&cvattr, PTHREAD_PROCESS_SHARED);
    Pthread_mutex_init(&shr_buff->lock, &mattr);
    Pthread_cond_init(&shr_buff->full, &cvattr);
    Pthread_cond_init(&shr_buff->turn, &cvattr);
    pid = fork();
    if (pid == 0) {
        dinosaur();
        exit(0);
    }
    int i;
    for (i = 0; i < NUM_CAVEMEN; i++) {
        pid = fork();
        if (pid == 0) {
            caveman(i);
            exit(0);
        }
    }
    wait(NULL);
    pthread_exit(NULL);
}
