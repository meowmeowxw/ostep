#include <assert.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include "mythreads.h"

void Pthread_mutex_init(pthread_mutex_t *mutex,
                        const pthread_mutexattr_t *attr) {
    char msg[MAX_MSG_SIZE];
    int rc = pthread_mutex_init(mutex, attr);
    if (rc != 0) {
        strerror_r(rc, msg, MAX_MSG_SIZE);
        fprintf(stderr, "mutex_init error: %s\n", msg);
        exit(1);
    }
}

void Pthread_mutex_lock(pthread_mutex_t *m) {
    char msg[MAX_MSG_SIZE];
    int rc = pthread_mutex_lock(m);
    if (rc != 0) {
        strerror_r(rc, msg, MAX_MSG_SIZE);
        fprintf(stderr, "mutex_lock error: %s\n", msg);
        exit(1);
    }
}

void Pthread_mutex_unlock(pthread_mutex_t *m) {
    char msg[MAX_MSG_SIZE];
    int rc = pthread_mutex_unlock(m);
    if (rc != 0) {
        strerror_r(rc, msg, MAX_MSG_SIZE);
        fprintf(stderr, "mutex_unlock error: %s\n", msg);
        exit(1);
    }
}

void Pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr) {
    char msg[MAX_MSG_SIZE];
    int rc = pthread_cond_init(cond, attr);
    if (rc != 0) {
        strerror_r(rc, msg, MAX_MSG_SIZE);
        fprintf(stderr, "cond_init error: %s\n", msg);
        exit(1);
    }
}

void Pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex) {
    char msg[MAX_MSG_SIZE];
    int rc = pthread_cond_wait(cond, mutex);
    if (rc != 0) {
        strerror_r(rc, msg, MAX_MSG_SIZE);
        fprintf(stderr, "cond_wait error: %s\n", msg);
        exit(1);
    }
}

void Pthread_cond_signal(pthread_cond_t *cond) {
    char msg[MAX_MSG_SIZE];
    int rc = pthread_cond_signal(cond);
    if (rc != 0) {
        strerror_r(rc, msg, MAX_MSG_SIZE);
        fprintf(stderr, "cond_signal error: %s\n", msg);
        exit(1);
    }
}

void Pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                    void *(*start_routine)(void *), void *arg) {
    char msg[MAX_MSG_SIZE];
    int rc = pthread_create(thread, attr, start_routine, arg);
    if (rc != 0) {
        strerror_r(rc, msg, MAX_MSG_SIZE);
        fprintf(stderr, "create error: %s\n", msg);
        exit(1);
    }
}

void Pthread_join(pthread_t thread, void **value_ptr) {
    char msg[MAX_MSG_SIZE];
    int rc = pthread_join(thread, value_ptr);
    if (rc != 0) {
        strerror_r(rc, msg, MAX_MSG_SIZE);
        fprintf(stderr, "join error: %s\n", msg);
        exit(1);
    }
}

void Pthread_cond_broadcast(pthread_cond_t *cond) {
    char msg[MAX_MSG_SIZE];
    int rc = pthread_cond_broadcast(cond);
    if (rc != 0) {
        strerror_r(rc, msg, MAX_MSG_SIZE);
        fprintf(stderr, "cond_broadcast error: %s\n", msg);
        exit(1);
    }
}

void Pthread_mutex_destroy(pthread_mutex_t *mutex) {
    char msg[MAX_MSG_SIZE];
    int rc = pthread_mutex_destroy(mutex);
    if (rc != 0) {
        strerror_r(rc, msg, MAX_MSG_SIZE);
        fprintf(stderr, "mutex_destroy error: %s\n", msg);
        exit(1);
    }
}

void Pthread_cond_destroy(pthread_cond_t *cond) {
    char msg[MAX_MSG_SIZE];
    int rc = pthread_cond_destroy(cond);
    if (rc != 0) {
        strerror_r(rc, msg, MAX_MSG_SIZE);
        fprintf(stderr, "cond_destroy error: %s\n", msg);
        exit(1);
    }
}

void Pthread_mutexattr_init(pthread_mutexattr_t *attr) {
    char msg[MAX_MSG_SIZE];
    int rc = pthread_mutexattr_init(attr);
    if (rc != 0) {
        strerror_r(rc, msg, MAX_MSG_SIZE);
        fprintf(stderr, "mutexattr_init error: %s\n", msg);
        exit(1);
    }
}

void Pthread_condattr_init(pthread_condattr_t *attr) {
    char msg[MAX_MSG_SIZE];
    int rc = pthread_condattr_init(attr);
    if (rc != 0) {
        strerror_r(rc, msg, MAX_MSG_SIZE);
        fprintf(stderr, "condattr_init error: %s\n", msg);
        exit(1);
    }
}

void Pthread_mutexattr_setpshared(pthread_mutexattr_t *attr, int pshared) {
    char msg[MAX_MSG_SIZE];
    int rc = pthread_mutexattr_setpshared(attr, pshared);
    if (rc != 0) {
        strerror_r(rc, msg, MAX_MSG_SIZE);
        fprintf(stderr, "mutexattr_setpshared error: %s\n", msg);
        exit(1);
    }
}

void Pthread_condattr_setpshared(pthread_condattr_t *attr, int pshared) {
    char msg[MAX_MSG_SIZE];
    int rc = pthread_condattr_setpshared(attr, pshared);
    if (rc != 0) {
        strerror_r(rc, msg, MAX_MSG_SIZE);
        fprintf(stderr, "condattr_setpshared error: %s\n", msg);
        exit(1);
    }
}

