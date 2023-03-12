#include<pthread.h>

#ifndef SEMA_H_INCLUDED
#define SEMA_H_INCLUDED

typedef struct sem_t{
    // public:
    unsigned int count;
    pthread_cond_t condVar;// = PTHREAD_COND_INITIALIZER;
    pthread_mutex_t condLock;// = PTHREAD_MUTEX_INITIALIZER;
}sem_t;

void sem_wait(sem_t *sem){
    pthread_mutex_lock(&(sem->condLock));
    while((sem->count)==0){
        pthread_cond_wait(&(sem->condVar), &(sem->condLock));
    }
    --(sem->count);
    pthread_mutex_unlock(&(sem->condLock));
    return;
}

void sem_post(sem_t *sem){
    pthread_mutex_lock(&(sem->condLock));
    ++(sem->count);
    pthread_cond_signal(&(sem->condVar));
    pthread_mutex_unlock(&(sem->condLock));
    return;
}

void sem_init(sem_t *sem, int tmp, int initial_value){
    pthread_cond_init(&(sem->condVar),NULL);
    pthread_mutex_init(&(sem->condLock),NULL);
    sem->count = initial_value;
    return;
}

void sem_destroy(sem_t *sem){
    while(!(sem->count)){
        sem_post(sem);
    }
    return;
}

#endif
Footer
