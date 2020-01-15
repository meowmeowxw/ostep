/* DBGpthread.c */

/* a causa dell'uso di strerror_r
   dove incluso, compilare con -D_POSIX_C_SOURCE >= 200112L
*/

#ifndef _THREAD_SAFE
#define _THREAD_SAFE
#endif
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200112L
#else
#if _POSIX_C_SOURCE < 200112L
#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200112L
#endif
#endif

#include "printerror.h"
#include <errno.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* per strerror_r  and  memset */
#include <unistd.h>

/* #define DEBUG */

void DBGpthread_mutex_lock(pthread_mutex_t *AddrMutex, const char *stringMsg) {
    int rc;
#ifdef DEBUG
    if (stringMsg)
        printf("lock %s\n", stringMsg);
#endif
    rc = pthread_mutex_lock(AddrMutex);
    if (rc)
        PrintERROR_andExit(rc, "pthread_mutex_lock failed");
}

int DBGpthread_mutex_trylock(pthread_mutex_t *AddrMutex,
                             const char *stringMsg) {
    int rc;
#ifdef DEBUG
    if (stringMsg)
        printf("trylock %s\n", stringMsg);
#endif
    rc = pthread_mutex_trylock(AddrMutex);
    if (rc == EBUSY) {
#ifdef DEBUG
        if (stringMsg)
            printf("trylock %s do not acquire lock\n", stringMsg);
#endif
        return (rc);
    }
    if (rc)
        PrintERROR_andExit(rc, "pthread_mutex_lock failed");
    return (0);
}

void DBGpthread_mutex_unlock(pthread_mutex_t *AddrMutex,
                             const char *stringMsg) {
    int rc;
#ifdef DEBUG
    if (stringMsg)
        printf("unlock %s\n", stringMsg);
#endif
    rc = pthread_mutex_unlock(AddrMutex);
    if (rc)
        PrintERROR_andExit(rc, "pthread_mutex_unlock failed");
}

void DBGpthread_cond_wait(pthread_cond_t *AddrCond, pthread_mutex_t *AddrMutex,
                          const char *stringMsg) {
    int rc;
#ifdef DEBUG
    if (stringMsg)
        printf("wait %s\n", stringMsg);
#endif
    rc = pthread_cond_wait(AddrCond, AddrMutex);
    if (rc)
        PrintERROR_andExit(rc, "pthread_cond_wait failed");
#ifdef DEBUG
    if (stringMsg)
        printf("wait exit %s\n", stringMsg);
#endif
}

void DBGpthread_cond_signal(pthread_cond_t *AddrCond, const char *stringMsg) {
    int rc;
#ifdef DEBUG
    if (stringMsg)
        printf("signal %s\n", stringMsg);
#endif
    rc = pthread_cond_signal(AddrCond);
    if (rc)
        PrintERROR_andExit(rc, "pthread_cond_signal failed");
}

void DBGpthread_cond_broadcast(pthread_cond_t *AddrCond,
                               const char *stringMsg) {
    int rc;
#ifdef DEBUG
    if (stringMsg)
        printf("broadcast %s\n", stringMsg);
#endif
    rc = pthread_cond_broadcast(AddrCond);
    if (rc)
        PrintERROR_andExit(rc, "pthread_cond_broadcast failed");
}

void DBGpthread_mutex_init(pthread_mutex_t *mutex,
                           const pthread_mutexattr_t *attr,
                           const char *stringMsg) {
    int rc;
#ifdef DEBUG
    if (stringMsg) {
        printf("pthread_mutex_init %s\n", stringMsg);
        fflush(stdout);
    }
#endif
    rc = pthread_mutex_init(mutex, attr);
    if (rc)
        PrintERROR_andExit(rc, "pthread_mutex_init failed");
}

void DBGpthread_mutex_destroy(pthread_mutex_t *mutex, const char *stringMsg) {
    int rc;
#ifdef DEBUG
    if (stringMsg) {
        printf("pthread_mutex_destroy %s\n", stringMsg);
        fflush(stdout);
    }
#endif
    rc = pthread_mutex_destroy(mutex);
    if (rc)
        PrintERROR_andExit(rc, "pthread_mutex_init failed");
}

void DBGpthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr,
                          const char *stringMsg) {
    int rc;
#ifdef DEBUG
    if (stringMsg) {
        printf("pthread_cond_init %s\n", stringMsg);
        fflush(stdout);
    }
#endif
    rc = pthread_cond_init(cond, attr);
    if (rc)
        PrintERROR_andExit(rc, "pthread_cond_init failed");
}

void DBGpthread_cond_destroy(pthread_cond_t *cond, const char *stringMsg) {
    int rc;
#ifdef DEBUG
    if (stringMsg) {
        printf("pthread_cond_destroy %s\n", stringMsg);
        fflush(stdout);
    }
#endif
    rc = pthread_cond_destroy(cond);
    if (rc)
        PrintERROR_andExit(rc, "pthread_cond_init failed");
}

void DBGsleep(unsigned int NumSeconds, const char *stringMsg) {
#ifdef DEBUG
    if (stringMsg)
        printf("sleep %s\n", stringMsg);
#endif
    sleep(NumSeconds);
}

void DBGnanosleep(unsigned int NumNanoSeconds, const char *stringMsg) {
    int rc, myerrno;
    struct timespec req, res;
#ifdef DEBUG
    if (stringMsg)
        printf("nanosleep %s\n", stringMsg);
#endif
    res.tv_sec = 0;
    res.tv_nsec = NumNanoSeconds;
    do {
        req = res;
        rc = nanosleep(&req, &res);
        myerrno = errno;
    } while ((rc < 0) && (myerrno == EINTR));

    if (rc < 0) {
        PrintERROR_andExit(myerrno, "nanosleep failed");
    }
}
