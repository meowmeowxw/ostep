#include <inttypes.h>
#include <mythreads.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SYNC_MAX 3 /* numero thread per ciascuno dei 2 tipi */
#define INDEX_A 0
#define INDEX_B 1

pthread_mutex_t mutex;
pthread_cond_t cond_sync, cond_exit;
int sync_count = 0;
intptr_t V[2] = {-1, -1};

void exit_as_first(intptr_t index, int index_type, int index_other_type,
                   char *Tlabel) {
    char debugmsg[1024];
    /* io sono il primo di coppia ad uscire */
    V[index_type] = index;
    /* avviso che sto attendendo secondo di coppia */
    sprintf(debugmsg, "%s_primodicoppia", Tlabel);
    Pthread_cond_signal(&cond_sync);
    /* attendo arrivo secondo di coppia */
    Pthread_cond_wait(&cond_exit, &mutex);
    /* ora posso leggere chi e' il secondo di coppia */
    printf("sono %" PRIiPTR " esco assieme a %" PRIiPTR "\n", index,
           V[index_other_type]);
    fflush(stdout);
    /* dico al secondo di coppia che ora possiamo uscire */
    Pthread_cond_signal(&cond_exit);
}

void exit_as_second(intptr_t index, int index_type, int index_other_type,
                    char *Tlabel) {
    char debugmsg[1024];

    V[index_type] = index;
    /* ora posso leggere chi e' il primo di coppia */
    printf("I am %" PRIiPTR " exit with %" PRIiPTR "\n", index,
           V[index_other_type]);
    fflush(stdout);
    /* avviso che sono arrivato come secondo di coppia */
    sprintf(debugmsg, "%s_second_pair", Tlabel);
    Pthread_cond_signal(&cond_exit);
    /* attendo conferma da primo di coppia che lui abbia letto mio index */
    Pthread_cond_wait(&cond_exit, &mutex);
    /* prima di uscire resetto gli indici degli uscenti */
    V[INDEX_A] = -1;
    V[INDEX_B] = -1;
}

void barrier(int index, char thread_type, int index_type,
             int index_other_type) {
    char Tlabel[1024];
    char debugmsg[1024];
    if (thread_type == 'A') {
        sprintf(Tlabel, "A%d", index);
    } else {
        sprintf(Tlabel, "B%d", index);
    }

    // Block sync count access
    // sprintf(debugmsg, "%s_start", Tlabel);
    Pthread_mutex_lock(&mutex);
    sync_count++;
    /* controlla se deve aspettare o no */
    if (sync_count < SYNC_MAX + 1) {
        Pthread_cond_wait(&cond_sync, &mutex);
    }
    /* aspetta di uscire */
    while (1) {
        /* guardo se sono il primo della coppia e
         * se sono del giusto thread_type */
        if (V[index_type] == -1) { /* sono del thread_type giusto */

            if (V[index_other_type] == -1) { /* sono il primo di coppia */
                exit_as_first(index, index_type, index_other_type, Tlabel);
            } else { /* sono il secondo di coppia ad uscire */
                exit_as_second(index, index_type, index_other_type, Tlabel);
                /* dico ad un altro in coda che puo' uscire */
                Pthread_cond_signal(&cond_sync);
            }
            break; /* esco dal while */
        } else {
            Pthread_cond_signal(&cond_sync);
            Pthread_cond_wait(&cond_sync, &mutex);
        }
    }
    // exit_as_first(index, index_type, index_other_type, Tlabel);
    sprintf(debugmsg, "%s_exit", Tlabel);
    Pthread_mutex_unlock(&mutex);
    return;
}

void *Thread(void *arg) {
    char thread_type;
    int index_type, index_other_type;
    intptr_t index;
    index = *(int *)arg;
    if (index < SYNC_MAX) {
        thread_type = 'A';
        index_type = INDEX_A;
        index_other_type = INDEX_B;
    } else {
        thread_type = 'B';
        index_type = INDEX_B;
        index_other_type = INDEX_A;
    }
    barrier(index, thread_type, index_type, index_other_type);
    pthread_exit(NULL);
}

int main() {
    int i;
    int x[2 * SYNC_MAX];
    pthread_t th[2 * SYNC_MAX];
    Pthread_cond_init(&cond_sync, NULL);
    Pthread_cond_init(&cond_exit, NULL);
    Pthread_mutex_init(&mutex, NULL);

    /* index < SYNC_MAX thread_type A
     * index >= SYNC_MAX thread_type B */
    for (i = 0; i < 2 * SYNC_MAX; i++) {
        x[i] = i;
        Pthread_create(&(th[i]), NULL, Thread, &x[i]);
    }
    for (i = 0; i < 2 * SYNC_MAX; i++) {
        Pthread_join(th[i], NULL);
    }

    Pthread_mutex_destroy(&mutex);
    Pthread_cond_destroy(&cond_sync);
    Pthread_cond_destroy(&cond_exit);
    pthread_exit(NULL);
}
