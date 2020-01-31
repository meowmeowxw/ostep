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

#define NUM_SOBERS 4
#define NUM_DRUNKS 2
#define MAX 2

typedef struct __shared_buffer {
    pthread_mutex_t mutex,
                    mutex_ticket;
    pthread_cond_t ready,
                   full,
                   start,
                   end;
    int is_full,
        is_empty,
        ticket,
        ticket_go;
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
        // while the funicular is not empty we can't let other passenger get
        // inside
        while (shr_buff->is_empty != 0) {
            Pthread_cond_wait(&shr_buff->start, &shr_buff->mutex);
        }
        // at the end of the tour and when the funicular is empty we can
        // broadcast the status
        shr_buff->is_full = 0;
        Pthread_cond_broadcast(&shr_buff->ready);
        Pthread_mutex_unlock(&shr_buff->mutex);
    }
    return;
}

void drunk(int id) {
    int myticket;
    while (1) {
        Pthread_mutex_lock(&shr_buff->mutex_ticket);
        myticket = shr_buff->ticket++;
        printf("drunk %d take ticket %d\n", id, myticket);
        Pthread_mutex_unlock(&shr_buff->mutex_ticket);
    }
    return;
}

void sober(int id) {
    int myticket;
    while (1) {
        Pthread_mutex_lock(&shr_buff->mutex_ticket);
        myticket = shr_buff->ticket;
        shr_buff->ticket++;
        printf("sober %d take ticket %d\n", id, myticket);
        Pthread_mutex_unlock(&shr_buff->mutex_ticket);

        Pthread_mutex_lock(&shr_buff->mutex);
        // wait until is not my turn, or the funicular is full or my ticket
        // is not valid yet
        while (myticket != shr_buff->ticket_go || shr_buff->is_full >= MAX) {
            printf("sober %d wait...\n", id);
            Pthread_cond_wait(&shr_buff->ready, &shr_buff->mutex);
        }
        shr_buff->is_full++;
        shr_buff->ticket_go++;
        printf("sober %d with ticket %d get on funicular\n", id, myticket);
        // if the cabine is full then we need to signal the funicular,
        // otherwise, other passengers can enjoy the tour with us and we let
        // them know
        if (shr_buff->is_full == MAX) {
            Pthread_cond_signal(&shr_buff->full);
        } else {
            Pthread_cond_broadcast(&shr_buff->ready);
        }

        // wait until the tour is ended and we signal to the funicular that we
        // got out
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
    int shmfd, rc;
    int shared_seg_size = sizeof(shared_buffer);
    int x[NUM_SOBERS];
    pthread_t th[NUM_SOBERS];
    pthread_t fun;
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
    shr_buff->ticket = 0;
    shr_buff->ticket_go = 0;

    Pthread_mutexattr_init(&mattr);
    Pthread_condattr_init(&cvattr);
    Pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_SHARED);
    Pthread_condattr_setpshared(&cvattr, PTHREAD_PROCESS_SHARED);
    Pthread_mutex_init(&shr_buff->mutex, &mattr);
    Pthread_mutex_init(&shr_buff->mutex_ticket, &mattr);
    Pthread_cond_init(&shr_buff->ready, &cvattr);
    Pthread_cond_init(&shr_buff->full, &cvattr);
    Pthread_cond_init(&shr_buff->start, &cvattr);
    Pthread_cond_init(&shr_buff->end, &cvattr);
    
    switch(fork()) {
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
        switch(fork()) {
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
    wait(NULL);
    return 0;
}
