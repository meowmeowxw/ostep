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

#define NUM_ALPHA 5
#define NUM_BETA 4

typedef struct __shared_buffer {
    pthread_mutex_t mutex;
    pthread_mutex_t mutex_ticket;
    pthread_cond_t run[2];
    int is_running;
    int ticket[2];
    int ticket_go[2];
    char groups[2];
} shared_buffer;

shared_buffer *shr_buff;

void runner(int id) {
    int group = id % 2;
    int other_group = (group + 1) % 2;
    int myticket;
    while (1) {
        Pthread_mutex_lock(&shr_buff->mutex_ticket);
        myticket = shr_buff->ticket[group]++;
        //printf("runner %d of group %c ticket %d\n", 
        //  id, groups[group], myticket);
        Pthread_mutex_unlock(&shr_buff->mutex_ticket);
         
        Pthread_mutex_lock(&shr_buff->mutex);
        while (myticket != shr_buff->ticket_go[group] || 
                shr_buff->is_running == 1) {
            Pthread_cond_wait(&shr_buff->run[group], &shr_buff->mutex);
        }
        shr_buff->is_running = 1;
        //printf("runner %d of group %c ticket %d runs\n", 
        //  id, groups[group], myticket);
        Pthread_mutex_unlock(&shr_buff->mutex);
        sleep(1);
        Pthread_mutex_lock(&shr_buff->mutex);
        shr_buff->ticket_go[other_group]++;
        shr_buff->is_running = 0;
        printf("runner %d of group %c ticket %d finished\n", 
                id, shr_buff->groups[group], myticket);
        Pthread_cond_broadcast(&shr_buff->run[other_group]);
        Pthread_mutex_unlock(&shr_buff->mutex);
    }
    return;
}

int main(int argc, char **argv) {
    // Without error checks :/
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
    
    shr_buff->is_running = 0;
    shr_buff->ticket[0] = 0;
    shr_buff->ticket[1] = 0;
    shr_buff->ticket_go[0] = 0;
    shr_buff->ticket_go[1] = -1;
    shr_buff->groups[0] = 'A';
    shr_buff->groups[1] = 'B';

    pthread_mutexattr_init(&mattr);
    pthread_condattr_init(&cvattr);
    pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_SHARED);
    pthread_condattr_setpshared(&cvattr, PTHREAD_PROCESS_SHARED);
    Pthread_mutex_init(&shr_buff->mutex, &mattr);
    Pthread_mutex_init(&shr_buff->mutex_ticket, &mattr);
    Pthread_cond_init(&shr_buff->run[0], &cvattr);
    Pthread_cond_init(&shr_buff->run[1], &cvattr);
    for (i = 0; i < NUM_BETA + NUM_ALPHA; i++) {
        switch (fork()) {
        case -1:
            fprintf(stderr, "error fork\n");
            exit(0);
            break;
        case 0:
            runner(i);
            exit(0);
            break;
        default:
            break;
        }
    }
    
    wait(NULL);
    pthread_exit(NULL);
}

