#include <pthread.h>

#ifndef __MYTHREADS_h__
#define __MYTHREADS_h__

#ifndef MAX_MSG_SIZE
#define MAX_MSG_SIZE 1024
#endif  // __MAX_MSG_SIZE__

void Pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                    void *(*start_routine)(void *), void *arg);
void Pthread_mutex_init(pthread_mutex_t *mutex,
                        const pthread_mutexattr_t *attr);
void Pthread_mutex_lock(pthread_mutex_t *m);
void Pthread_mutex_unlock(pthread_mutex_t *m);
void Pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr);
void Pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
void Pthread_cond_signal(pthread_cond_t *cond);
void Pthread_join(pthread_t thread, void **value_ptr);
void Pthread_cond_broadcast(pthread_cond_t *cond);
void Pthread_mutex_destroy(pthread_mutex_t *mutex);
void Pthread_cond_destroy(pthread_cond_t *cond);
void Pthread_mutexattr_init(pthread_mutexattr_t *attr);
void Pthread_condattr_init(pthread_condattr_t *attr);
void Pthread_mutexattr_setpshared(pthread_mutexattr_t *attr, int pshared);
void Pthread_condattr_setpshared(pthread_condattr_t *attr, int pshared);

#endif // __MYTHREADS_h__

