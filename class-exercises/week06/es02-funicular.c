#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <mythreads.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define NUM_SOBERS 4
#define NUM_DRUNKS 2
#define MAX 2

typedef struct __shared_buffer {
    pthread_mutex_t mutex;
    pthread_cond_t ready;
    pthread_cond_t full;
    pthread_cond_t start;
    pthread_cond_t end;
    int is_full;
    int is_empty;
} shared_buffer;

shared_buffer *shr_buff;

void funicular() {
    sleep(1);
    while (1) {
        Pthread_mutex_lock(&shr_buff->mutex);
        while (shr_buff->is_full < MAX) {
            Pthread_cond_wait(&shr_buff->full, &shr_buff->mutex);
        }
        Pthread_mutex_unlock(&shr_buff->mutex);
        puts("\nstart of the panoramic tour");
        sleep(1);
        puts("we're at the top");
        sleep(1);
        puts("tour ended\n");
        Pthread_mutex_lock(&shr_buff->mutex);
        shr_buff->is_empty = MAX;
        Pthread_cond_broadcast(&shr_buff->end);
        while (shr_buff->is_empty != 0) {
            Pthread_cond_wait(&shr_buff->start, &shr_buff->mutex);
        }
        shr_buff->is_full = 0;
        Pthread_cond_broadcast(&shr_buff->ready);
        Pthread_mutex_unlock(&shr_buff->mutex);
    }
    return;
}

void drunk(int id) {
    while (1) {
        Pthread_mutex_lock(&shr_buff->mutex);
        while (shr_buff->is_full != 0 || shr_buff->is_full >= 2) {
            //printf("drunk %d waits...\n", id);
            Pthread_cond_wait(&shr_buff->ready, &shr_buff->mutex);
        }
        shr_buff->is_full += 2;
        printf("drunk %d get on funicular\n", id);
        Pthread_cond_signal(&shr_buff->full);
        Pthread_cond_wait(&shr_buff->end, &shr_buff->mutex);
        printf("drunk %d left\n", id);
        shr_buff->is_empty -= 2;
        Pthread_cond_signal(&shr_buff->start);
        Pthread_mutex_unlock(&shr_buff->mutex);
    }
    return;
}

void sober(int id) {
    while (1) {
        Pthread_mutex_lock(&shr_buff->mutex);
        while (shr_buff->is_full >= MAX) {
            //printf("sober %d waits...\n", id);
            Pthread_cond_wait(&shr_buff->ready, &shr_buff->mutex);
        }
        printf("sober %d get on funicular\n", id);
        shr_buff->is_full++;
        if (shr_buff->is_full == MAX) {
            Pthread_cond_signal(&shr_buff->full);
        } else {
            Pthread_cond_broadcast(&shr_buff->ready);
        }
        Pthread_cond_wait(&shr_buff->end, &shr_buff->mutex);
        printf("sober %d left\n", id);
        shr_buff->is_empty--;
        Pthread_cond_signal(&shr_buff->start);
        Pthread_mutex_unlock(&shr_buff->mutex);
    }
    return;
}

int main(int argc, char **argv) {
    setbuf(stdout, NULL);
    int i;
    int shmfd;
    int rc;
    int shared_seg_size = sizeof(shared_buffer);
    pthread_mutexattr_t mattr;
    pthread_condattr_t cvattr;

    shmfd = shm_open("/tmp", O_CREAT | O_RDWR, S_IRWXU);
    assert(shmfd >= 0);
    rc = ftruncate(shmfd, shared_seg_size);
    assert(rc == 0);

    shr_buff = (shared_buffer *)mmap(
        NULL, shared_seg_size, PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);
    assert(shr_buff != MAP_FAILED);
    shr_buff->is_full = 0;
    shr_buff->is_empty = 0;

    Pthread_mutexattr_init(&mattr);
    Pthread_condattr_init(&cvattr);
    Pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_SHARED);
    Pthread_condattr_setpshared(&cvattr, PTHREAD_PROCESS_SHARED);
    Pthread_mutex_init(&shr_buff->mutex, &mattr);
    Pthread_cond_init(&shr_buff->ready, &cvattr);
    Pthread_cond_init(&shr_buff->full, &cvattr);
    Pthread_cond_init(&shr_buff->start, &cvattr);
    Pthread_cond_init(&shr_buff->end, &cvattr);

    switch (fork()) {
    case -1:
        fprintf(stderr, "error fork\n");
        exit(0);
        break;
    case 0:
        funicular();
        exit(0);
        break;
    default:
        break;
    }
    for (i = 0; i < NUM_SOBERS; i++) {
        switch (fork()) {
        case -1:
            fprintf(stderr, "error fork\n");
            exit(0);
            break;
        case 0:
            sober(i);
            exit(0);
            break;
        default:
            break;
        }
    }
    for (i = 0; i < NUM_DRUNKS; i++) {
        switch (fork()) {
        case -1:
            fprintf(stderr, "error fork\n");
            exit(0);
            break;
        case 0:
            drunk(i);
            exit(0);
            break;
        default:
            break;
        }
    }
    wait(NULL);
    return 0;
}
