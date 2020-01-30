#ifndef __MYTHREADS_h__
#define __MYTHREADS_h__

#include <assert.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

void Pthread_mutex_init(pthread_mutex_t *mutex,
                        const pthread_mutexattr_t *attr) {
    int rc = pthread_mutex_init(mutex, attr);
    assert(rc == 0);
}

void Pthread_mutex_lock(pthread_mutex_t *m) {
    int rc = pthread_mutex_lock(m);
    assert(rc == 0);
}

void Pthread_mutex_unlock(pthread_mutex_t *m) {
    int rc = pthread_mutex_unlock(m);
    assert(rc == 0);
}

void Pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr) {
    int rc = pthread_cond_init(cond, attr);
    assert(rc == 0);
}

void Pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex) {
    int rc = pthread_cond_wait(cond, mutex);
    assert(rc == 0);
}

void Pthread_cond_signal(pthread_cond_t *cond) {
    int rc = pthread_cond_signal(cond);
    assert(rc == 0);
}

void Pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                    void *(*start_routine)(void *), void *arg) {
    int rc = pthread_create(thread, attr, start_routine, arg);
    assert(rc == 0);
}

void Pthread_join(pthread_t thread, void **value_ptr) {
    int rc = pthread_join(thread, value_ptr);
    assert(rc == 0);
}

void Pthread_cond_broadcast(pthread_cond_t *cond) {
    int rc = pthread_cond_broadcast(cond);
    assert(rc == 0);
}

void Pthread_mutex_destroy(pthread_mutex_t *mutex) {
    int rc = pthread_mutex_destroy(mutex);
    assert(rc == 0);
}

void Pthread_cond_destroy(pthread_cond_t *cond) {
    int rc = pthread_cond_destroy(cond);
    assert(rc == 0);
}

void Pthread_mutexattr_init(pthread_mutexattr_t *attr) {
    int rc = pthread_mutexattr_init(attr);
    assert(rc == 0);
}

void Pthread_condattr_init(pthread_condattr_t *attr) {
    int rc = pthread_condattr_init(attr);
    assert(rc == 0);
}

void Pthread_mutexattr_setpshared(pthread_mutexattr_t *attr, int pshared) {
    int rc = pthread_mutexattr_setpshared(attr, pshared);
    assert(rc == 0);
}

void Pthread_condattr_setpshared(pthread_condattr_t *attr, int pshared) {
    int rc = pthread_condattr_setpshared(attr, pshared);
    assert(rc == 0);
}

#endif // __MYTHREADS_h__

