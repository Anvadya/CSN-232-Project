#ifndef SEMA_H_INCLUDED
#define SEMA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>


typedef struct sem_t{
    unsigned int count;
    int request[2], response[2];
    unsigned int waitingProcesses;
}sem_t; 


void sem_wait(sem_t *sem){
    int request = 1; int reply;
    write(sem->request[1], &request, sizeof(int));
    read(sem->response[0], &reply, sizeof(int));
    return;
}

void sem_post(sem_t *sem){
    int request = 0;
    write(sem->request[1], &request, sizeof(int));
    return;
}

void sem_init(sem_t *sem, int tmp, int initial_value){
    sem->count = initial_value;
    sem->waitingProcesses = 0;
    pipe(sem->request);
    pipe(sem->response);
    if(fork()==0){
        int reply = 1; int req;
        while(1){
            read(sem->request[0], &req, sizeof(int));
            if(req == -1) exit(0);
            if(req){
                if(sem->count){
                    --sem->count;
                    write(sem->response[1], &reply, sizeof(int));
                }
                else{
                    ++sem->waitingProcesses;
                }
            }
            else{
                ++sem->count;
            }
            if(sem->count>0 && sem->waitingProcesses){
                --sem->count;
                --sem->waitingProcesses;
                write(sem->response[1], &reply, sizeof(int));
            }
        }
    }
    return;
}

void sem_destroy(sem_t *sem){
    while(sem->waitingProcesses){
        sem_post(sem);
    }
    int request = -1;
    write(sem->request[1], &request, sizeof(int));
    return;
}

#endif