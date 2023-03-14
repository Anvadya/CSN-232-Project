#include <stdio.h>
#include <pthread.h>
#include "messagePassing.h"
//Including the header file messagePassing.h (Refer to the file for its documentation)

#define NO_OF_OPERATIONS 8
//This is the number of independent operations a thread executes sequentially
//For eg, a thread decompressing some data, computing some values and then preparing a report will be counted as 3 operations

#define MAX(x, y) (((x) > (y)) ? (x) : (y)) //The max function implemneted as a macro
#define MIN(x, y) (((x) < (y)) ? (x) : (y)) //The min function implemented as a macro

int operations[NO_OF_PROCESSES+1][NO_OF_OPERATIONS];
//This array stores the function at each point the data dependency relation for each operation
//operations[i][j] store the pid of the process from which data is required for completing step j of process i
//so if operations[i][j] = i, then only the thread's local data is required
//but if operations[i][j] = k (k is not i) then it means thaat data from thread k is:
//   a) required (has to be read) if k is a negative number
//   b) has to be sent (write) if k is a positive number

void *process();


//DRIVER CODE
int main(){
    initialise(); 
    //Initialising the pipes (refer the header file)
    pthread_t process_thr[NO_OF_PROCESSES]; //Initialising threads
    int pid[NO_OF_PROCESSES+1]; //This array will store the pid's
    
    //Manaually setting the operation dependency
    //(The program is required to give this information to the OS)
    for(int i = 0; i < NO_OF_PROCESSES+1; ++i){
        for(int j = 0; j < NO_OF_OPERATIONS; ++j){
            operations[i][j] = i;
        }
    }
    operations[1][4] = 2;
    operations[2][2] = -1;
    operations[2][4] = 1;
    operations[1][6] = -2;
    //IMPORTANT; MESSAGE PASSING EDGES BETWEEN TWO THREADS MUST NOT INTERSECT EACH OTHER
    //(To avoid cycles in the graph, the graph ought to be a DAG)
    //Cycles can lead to Deadlocks (to be ensured by the user, Ostrich algorithm is used)
    
    //Initialising the pids
    for(int i = 0; i < NO_OF_PROCESSES+1; ++i) pid[i] = i;
    //Creating threads
    for(int i = 0; i < NO_OF_PROCESSES; ++i){
        pthread_create(&process_thr[i], NULL, (void *)process, (void *)&pid[i+1]);
    }
    //Merging threads (to avoid zombie threads)
    for(int i = 0; i < NO_OF_PROCESSES; ++i){
        pthread_join(process_thr[i], NULL);
    }
}   

//Format of messages sent/received:
// Message 1: The data to be sent
// Message 2: The time at which the message has been sent (logical clock's value of that thread/process)

//The blueprint of the process
void *process(void *args){
    //Initialisation of the thread
    int pid = *((int *)args); //reading the pid of the process
    int clock_timer = 0; //Starting the local timer of the thread
    //this represents the time of its logical clock
    //NB: Logical clock is very different from the physical clock used in computers
    int data, tmpTime;
    //data: stores the data to be read (sent by an external thread/process)
    //tmpTime: stores the timestamp at which another process/thread sends the message
    
    //Starting execcution of the thread
    for(int i = 0; i < NO_OF_OPERATIONS; ++i){
        if(operations[pid][i] == pid) ++clock_timer; //Incrementing clock timer
        else{
            if(operations[pid][i] > 0){ //data has to be sent to another thread/process
                sendMessage(pid, operations[pid][i], pid); //Sending data
                sendMessage(pid, operations[pid][i], clock_timer); //Sending timestamp
                printf("Writing data from %d to %d at time %d\n", pid, operations[pid][i], clock_timer);
                fflush(stdout);
                ++clock_timer; //Incrementing clock timer
            }
            else{
                data = readMessage(-operations[pid][i], pid); //Reading the message
                //Note: The process goes to a wait state if the message has not yet reached the pipe (intended behaviour to guarantee correctness)
                printf("Read data from %d by %d, data = %d\n", -operations[pid][i], pid, data);
                fflush(stdout);
                tmpTime = readMessage(-operations[pid][i], pid); //Reading the timestamp
                printf("Read clock time from %d by %d, clock time = %d\n", -operations[pid][i], pid, tmpTime);
                fflush(stdout);
                ++tmpTime;
                clock_timer = MAX(clock_timer, tmpTime);//Updating the value of the thread's logical clock
                ++clock_timer; //Incrementing the value of the logical clock
            }
        }
        printf("Clock for process: %d is %d\n", pid, clock_timer);
        fflush(stdout);
    }
}
