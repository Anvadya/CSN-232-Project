#include <stdio.h>
#include <pthread.h>
#include "messagePassing.h"

#define NO_OF_OPERATIONS 8

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

int operations[NO_OF_PROCESSES+1][NO_OF_OPERATIONS];

void *process();

int main(){
    initialise();
    pthread_t process_thr[NO_OF_PROCESSES];
    int pid[NO_OF_PROCESSES+1];
    for(int i = 0; i < NO_OF_PROCESSES+1; ++i){
        for(int j = 0; j < NO_OF_OPERATIONS; ++j){
            operations[i][j] = i;
        }
    }
    operations[1][4] = 2;
    operations[2][2] = -1;
    operations[2][4] = 1;
    operations[1][6] = -2;
    for(int i = 0; i < NO_OF_PROCESSES+1; ++i) pid[i] = i;
    for(int i = 0; i < NO_OF_PROCESSES; ++i){
        pthread_create(&process_thr[i], NULL, (void *)process, (void *)&pid[i+1]);
    }
    for(int i = 0; i < NO_OF_PROCESSES; ++i){
        pthread_join(processs_thr[i], NULL);
    }
}   

void *process(void *args){
    // printf("Start\n"); fflush(stdout);
    int pid = *((int *)args);
    int clock_timer = 0;
    int data, tmpTime;
    for(int i = 0; i < NO_OF_OPERATIONS; ++i){
        if(operations[pid][i] == pid) ++clock_timer;
        else{
            if(operations[pid][i] > 0){
                sendMessage(pid, operations[pid][i], pid);
                sendMessage(pid, operations[pid][i], clock_timer);
                printf("Writing data from %d to %d at time %d\n", pid, operations[pid][i], clock_timer);
                fflush(stdout);
                ++clock_timer;
            }
            else{
                data = readMessage(-operations[pid][i], pid);
                printf("Read data from %d by %d, data = %d\n", -operations[pid][i], pid, data);
                fflush(stdout);
                tmpTime = readMessage(-operations[pid][i], pid);
                printf("Read clock time from %d by %d, clock time = %d\n", -operations[pid][i], pid, tmpTime);
                fflush(stdout);
                ++tmpTime;
                clock_timer = MAX(clock_timer, tmpTime);
                ++clock_timer;
            }
        }
        printf("Clock for process: %d is %d\n", pid, clock_timer);
        fflush(stdout);
    }
}
