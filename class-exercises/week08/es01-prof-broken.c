#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <mythreads.h>

pthread_mutex_t mutex;
pthread_cond_t hurt;
pthread_cond_t healed;
pthread_cond_t all_present;
int is_hurt = 0;
int is_healed = 1;
int are_all_present = 0;

void *professor(void *arg) {
    while (1) {
        puts("\nprofessor is teaching");
        sleep(4);
        Pthread_mutex_lock(&mutex);
        is_hurt = 1;
        is_healed = 0;
        puts("\nprofessor is hurted");
        Pthread_cond_broadcast(&hurt);
        while (is_healed != 1) {
            Pthread_cond_wait(&healed, &mutex);
        }
        puts("professor is healed");
        sleep(1);
    }
    return NULL;
}

void *exorcist(void *arg) {
    while (1) {
        Pthread_mutex_lock(&mutex);
        while (is_hurt != 1) {
            Pthread_cond_wait(&hurt, &mutex);
        }
        are_all_present++;
        puts("exorcist arrived");
        if (are_all_present == 2) {
            sleep(2);
            is_healed = 1;
            is_hurt = 0;
            Pthread_cond_signal(&healed);
            Pthread_cond_signal(&all_present);
            Pthread_mutex_unlock(&mutex);
        } else {
            while (are_all_present != 2) {
                Pthread_cond_wait(&all_present, &mutex);
            }
            are_all_present = 0;
            Pthread_mutex_unlock(&mutex);
        }
    }
    return NULL;
}

void *doctor(void *arg) {
    while (1) {
        Pthread_mutex_lock(&mutex);
        while (is_hurt != 1) {
            Pthread_cond_wait(&hurt, &mutex);
        }
        are_all_present++;
        puts("doctor arrived");
        if (are_all_present == 2) {
            sleep(2);
            is_healed = 1;
            is_hurt = 0;
            Pthread_cond_signal(&healed);
            Pthread_cond_signal(&all_present);
            Pthread_mutex_unlock(&mutex);
        } else {
            while (are_all_present != 2) {
                Pthread_cond_wait(&all_present, &mutex);
            }
            are_all_present = 0;
            Pthread_mutex_unlock(&mutex);
        }
    }
    return NULL;
}

int main(int argc, char **argv) {
    int i;
    pthread_t prof;
    pthread_t exor;
    pthread_t doct;
    Pthread_mutex_init(&mutex, NULL);
    Pthread_cond_init(&hurt, NULL);
    Pthread_cond_init(&healed, NULL);
    Pthread_cond_init(&all_present, NULL);
    Pthread_create(&prof, NULL, professor, NULL);
    Pthread_create(&exor, NULL, exorcist, NULL);
    Pthread_create(&prof, NULL, doctor, NULL);
    Pthread_join(prof, NULL);
    Pthread_join(exor, NULL);
    Pthread_join(prof, NULL);
	return 0;
}

