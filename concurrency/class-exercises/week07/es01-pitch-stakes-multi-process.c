#include "printerror.h"

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <limits.h> /* for OPEN_MAX */
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

typedef struct sharedBuffer {
    int deviMartellare[2];
    int hoFatto[2];
    pthread_mutex_t mutex;
    pthread_cond_t wait0;
    pthread_cond_t wait1;
    pthread_cond_t aspetto;
} SharedBuffer;
SharedBuffer *P;

void *martello0(void) {
    int indice;
    int paliMartellati;
    indice = 0;
    paliMartellati = 1;
    while (1) {
        paliMartellati++;
        pthread_mutex_lock(&P->mutex);
        while (P->deviMartellare[indice] == 0) {
            pthread_cond_wait(&P->wait0, &P->mutex);
        }
        pthread_mutex_unlock(&P->mutex);
        printf("MARTELLO 0 INIZIA \n");
        sleep(2);
        printf("MARTELLO 0 FINISCE \n");
        pthread_mutex_lock(&P->mutex);
        P->hoFatto[indice] = 1;
        P->deviMartellare[indice] = 0;
        pthread_cond_signal(&P->aspetto);
        pthread_mutex_unlock(&P->mutex);
        if (paliMartellati == 4) {
            printf("DORMO UN POCO \n");
            sleep(5);
        }
    }
}

void *martello1(void) {
    int indice;
    int paliMartellati;
    indice = 1;
    paliMartellati = 0;
    while (1) {
        paliMartellati++;
        pthread_mutex_lock(&P->mutex);
        while (P->deviMartellare[indice] == 0) {
            pthread_cond_wait(&P->wait1, &P->mutex);
        }
        pthread_mutex_unlock(&P->mutex);
        printf("MARTELLO 1 INIZIA \n");
        sleep(2);
        printf("MARTELLO 1 FINISCE \n");
        pthread_mutex_lock(&P->mutex);
        P->hoFatto[indice] = 1;
        P->deviMartellare[indice] = 0;
        pthread_cond_signal(&P->aspetto);
        pthread_mutex_unlock(&P->mutex);
        if (paliMartellati == 4) {
            printf("DORMO UN POCO \n");
            sleep(7);
        }
    }
}

void *tieniPali(void) {
    int coppiePaliPresi = 0;
    while (1) {
        coppiePaliPresi++;
        printf("PRENDO I PALI. \n");
        sleep(2);
        pthread_mutex_lock(&P->mutex);
        P->deviMartellare[0] = 1;
        P->deviMartellare[1] = 1;
        pthread_cond_signal(&P->wait0);
        pthread_cond_signal(&P->wait1);
        while (P->hoFatto[0] == 0 || P->hoFatto[1] == 0) {
            pthread_cond_wait(&P->aspetto, &P->mutex);
        }
        P->hoFatto[0] = 0;
        P->hoFatto[1] = 0;
        pthread_mutex_unlock(&P->mutex);
        if (coppiePaliPresi == 5) {
            sleep(3);
        }
    }
}

int main(void) {
    int shmfd, rc;
    pid_t pid;
    int shared_seg_size = sizeof(SharedBuffer);
    pthread_mutexattr_t mattr;
    pthread_condattr_t cvattr;

    shmfd = shm_open("/pedala", O_CREAT /*| O_EXCL*/ | O_RDWR, S_IRWXU);
    if (shmfd < 0) {
        perror("In shm_open()");
        exit(1);
    }
    /* adjusting mapped file size (make room for the whole segment to map) */
    rc = ftruncate(shmfd, shared_seg_size);
    if (rc != 0) {
        perror("ftruncate() failed");
        exit(1);
    }

    P = (SharedBuffer *)mmap(NULL, shared_seg_size, PROT_READ | PROT_WRITE,
                             MAP_SHARED, shmfd, 0);
    if (P == MAP_FAILED)
        PrintERROR_andExit(errno, "mmap  failed");

    rc = pthread_mutexattr_init(&mattr);
    if (rc)
        PrintERROR_andExit(rc, "pthread_mutexattr_init  failed");
    rc = pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_SHARED);
    if (rc)
        PrintERROR_andExit(rc, "pthread_mutexattr_setpshared  failed");
    rc = pthread_condattr_init(&cvattr);
    if (rc)
        PrintERROR_andExit(rc, "pthread_condattr_init  failed");
    rc = pthread_condattr_setpshared(&cvattr, PTHREAD_PROCESS_SHARED);
    if (rc)
        PrintERROR_andExit(rc, "pthread_condattr_setpshared  failed");

    /*inizializzo strutture e  variabili per sincronizzazione*/
    rc = pthread_cond_init(&P->wait0, &cvattr);
    if (rc)
        PrintERROR_andExit(rc, "pthread_cond_init failed");
    rc = pthread_cond_init(&P->wait1, &cvattr);
    if (rc)
        PrintERROR_andExit(rc, "pthread_cond_init failed");
    rc = pthread_cond_init(&P->aspetto, &cvattr);
    if (rc)
        PrintERROR_andExit(rc, "pthread_cond_init failed");

    rc = pthread_mutex_init(&P->mutex, &mattr);
    if (rc)
        PrintERROR_andExit(rc, "pthread_mutex_init failed");

    P->deviMartellare[0] = 0;
    P->deviMartellare[1] = 0;
    P->hoFatto[0] = 0;
    P->hoFatto[1] = 0;

    pid = fork();
    if (pid < 0)
        PrintERROR_andExit(errno, "fork failed");
    else if (pid == 0) {
        tieniPali();
        exit(0);
    }
    pid = fork();
    if (pid < 0)
        PrintERROR_andExit(errno, "fork failed");
    else if (pid == 0) {
        martello0();
        exit(0);
    }
    pid = fork();
    if (pid < 0)
        PrintERROR_andExit(errno, "fork failed");
    else if (pid == 0) {
        martello1();
        exit(0);
    }
    wait(NULL);
    pthread_exit(NULL);
}
