#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include "DiskLock.h"

#define NO_OF_CPU 8
const int MEM_RANGE = N;

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

void *cpuProcess();

int main(){
    build(0, 0, N-1);
    pthread_t cpu[NO_OF_CPU];
    int pids[NO_OF_CPU];
    for(int i = 0; i < NO_OF_CPU; ++i) pids[i] = i+1;
    for(int i = 0; i < NO_OF_CPU; ++i){
        pthread_create(&cpu[i], NULL, (void *)cpuProcess, (void *)(&pids[i]));
    }
    for(int i = 0; i < NO_OF_CPU; ++i){
        pthread_join(cpu[i], NULL);
    }
    destroy();
    return 0;
}

void* cpuProcess(void *args){
    int pid = *((int *)args);
    int t1 = rand()%MEM_RANGE;
    int t2 = rand()%MEM_RANGE;
    int l = MIN(t1, t2);
    int r = MAX(t1, t2);
    // int l = 10, r = 25;
    printf("Trying to allocate memory range from: %d to %d by process: %d \n", l, r, pid);
    fflush(stdout);
    while(allocate(l,r,1) == 0) {printf("Failed to allocate memory to process %d, trying again......\n", pid); fflush(stdout); sleep(1);}
    printf("Memory allocated succesfully to process %d.\n", pid);
    fflush(stdout);
    sleep(1);
    deallocate(l, r);
    printf("Memory deallocated by process %d\n", pid);
    fflush(stdout);
}
