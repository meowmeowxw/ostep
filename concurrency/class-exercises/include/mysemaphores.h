#ifndef __MYSEMAPHORES_h__
#define __MYSEMAPHORES_h__

#include <semaphore.h>

void Sem_wait(sem_t *sem) {
    int rc = sem_wait(sem);
    assert(rc == 0);
}

void Sem_post(sem_t *sem) {
    int rc = sem_post(sem);
    assert(rc == 0);
}

void Sem_init(sem_t *sem, int pshared, unsigned int value) {
    int rc = sem_init(sem, pshared, value);
    assert(rc == 0);
}

#endif // __MYSEMAPHORES_h__
