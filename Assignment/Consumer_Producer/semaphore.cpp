// Semaphores used for various problems: implemented using pthread mutex and contion variable

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct {
    pthread_cond_t cond;
    pthread_mutex_t mtx;
    int value;
} Semaphore;


void SEM_INIT(Semaphore *ps,int n)
{
			ps->cond = PTHREAD_COND_INITIALIZER; 
    		ps->mtx  = PTHREAD_MUTEX_INITIALIZER; 
    		ps->value    = n;
}

void Semaphore_init(Semaphore * ps, int N) {
	printf("start\n");
    pthread_mutex_init(&ps->mtx,NULL);
    pthread_cond_init(&ps->cond,NULL);
    ps->value = N;
    printf("stop\n");
}


void Semaphore_destroy(Semaphore * ps) {
    pthread_cond_destroy(&ps->cond);
    pthread_mutex_destroy(&ps->mtx);
}


void Semaphore_wait(Semaphore * ps) {
    pthread_mutex_lock(&ps->mtx);
    while(ps->value == 0) {
        pthread_cond_wait(&ps->cond,&ps->mtx);
    }

    --(ps->value);
    pthread_mutex_unlock(&ps->mtx);
}

void Semaphore_signal(Semaphore * ps) {
    pthread_mutex_lock(&ps->mtx);
    ++(ps->value);
    pthread_mutex_unlock(&ps->mtx);
    pthread_cond_signal(&ps->cond);
}

