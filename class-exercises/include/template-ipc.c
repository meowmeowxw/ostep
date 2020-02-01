#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <mythreads.h>
#include <pthread.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h> 
#include <unistd.h>

typedef struct __shared_buffer {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int done;
} shared_buffer;

shared_buffer *shr_buff;

void child() {
    sleep(1);
    return;
}

int main(int argc, char **argv) {
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
    
    shr_buff->done = 0;
    pthread_mutexattr_init(&mattr);
    pthread_condattr_init(&cvattr);
    pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_SHARED);
    pthread_condattr_setpshared(&cvattr, PTHREAD_PROCESS_SHARED);
    Pthread_mutex_init(&shr_buff->mutex, &mattr);
    Pthread_cond_init(&shr_buff->cond, &cvattr);
    switch (fork()) {
    case -1:
        fprintf(stderr, "error fork\n");
        exit(0);
        break;
    case 0:
        child();
        exit(0);
        break;
    default:
        break;
    }
    wait(NULL);
    pthread_exit(NULL);
}

