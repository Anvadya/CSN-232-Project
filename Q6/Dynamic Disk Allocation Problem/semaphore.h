#ifndef SEMA_H_INCLUDED
#define SEMA_H_INCLUDED
//Inclusion Guard to prevent includinf thr header file multipole times

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>


//This struct defines the semaphore
typedef struct sem_t{
    unsigned int count; //The count variable of the semaphore
    int request[2], response[2]; //Arrays used for IPC by the semaphore
    unsigned int waitingProcesses; //No. of processes waiting on the semaphore
}sem_t; 

//MESSAGE FORMAT:
//Request of 0 means up request and request of 1 means down request

//This function performs the down operation on the semaphore
void sem_wait(sem_t *sem){
    int request = 1; int reply;
    //request = 1 -> for down
    write(sem->request[1], &request, sizeof(int));
    //Sending request
    read(sem->response[0], &reply, sizeof(int));
    //Waiting for response
    return;
}

//This function performs the up operation on the semaphore
void sem_post(sem_t *sem){
    int request = 0;
    //request = 0 -> for up
    write(sem->request[1], &request, sizeof(int));
    //Sending request
    return;
}

//This function initialises the semaphore
//Always destroy a semaphore after its work has been done
void sem_init(sem_t *sem, int tmp, int initial_value){
    sem->count = initial_value; //Initialising count 
    sem->waitingProcesses = 0; //Initialising no. of waiting processes
    //Creating pipes
    pipe(sem->request); 
    pipe(sem->response);
    
    //The process forks at this point
    //The child process handles all the requests
    //The parent process continues as before
    if(fork()==0){
        //Inside child process
        
        //Setting reply as 1 to indicate a succesful operation (i.e. the request can  be fulfilled)
        //The variable req stores the request sent by various processes/threads 
        int reply = 1; int req;
        
        //This process would run on an infinite loop till the semaphore is destroyed
        //It would keep processing requesta and then taking appropriate action
        while(1){
            read(sem->request[0], &req, sizeof(int)); //Reading the request
            //A request of:
            //  -1 indicates that the sem has to destroyed and this process can also stop now
            //  0 indicates a request to perform an up operation
            //  1 indicates a request to perform a down operation
            if(req == -1) exit(0);
            if(req){
                //This branch would be executed for down requests
                if(sem->count){
                    //A request can be immediately satisfied if the count is a positive number
                    --sem->count;
                    write(sem->response[1], &reply, sizeof(int));
                }
                else{
                    //if the count is 0 then we need to wait for some time until some other process performs an up operation
                    ++sem->waitingProcesses;
                }
            }
            else{
                //This branch would be executed for up requests
                ++sem->count; //Incrementing the count
            }
            //Checking whether any process/thread which was sleeping on this semaphore can be woken up now
            if(sem->count>0 && sem->waitingProcesses){
                //Waking up a sleeping process
                --sem->count;
                --sem->waitingProcesses;
                write(sem->response[1], &reply, sizeof(int));
            }
        }
    }
    return;
}

//This function is used when the usage of semaphore ends and we wish to destroy it
//IMPORTANT: The processes using semaphore will never terminate if the semaphores are not destroyed
//ALWAYS DESTROY SEMAPHORES BEFORE FINISHING EXECUTION
void sem_destroy(sem_t *sem){
    //Waking up all sleeping process on the semaphore
    while(sem->waitingProcesses){
        sem_post(sem);
    }
    
    //Sending a request to kill the process handling requests for this semaphore
    int request = -1;
    write(sem->request[1], &request, sizeof(int));
    return;
}

#endif

