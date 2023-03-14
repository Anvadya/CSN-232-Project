//<----------------------------------------HEADER FILE FOR Lamport Clock Based Synchronisation--------------------->
//This header file implements the ability to send and receive messages between processes
//The underlying construct used to achieve this is pipe

#ifndef MESSAGEP_H_INCLUDED
#define MESSAGEP_H_INCLUDED
//header inclusion guard to prevent the header file from being included multiple times

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

#define NO_OF_PROCESSES 2
//This is the number of independent threads executing on distributed computing cores
//Change the value of this symbol if you want more threads to execute the program across the cores

int filedes[NO_OF_PROCESSES+1][NO_OF_PROCESSES+1][2];
//This is the array for defining pipes
//filedes[i][j] represents a pipe which takes its input from process i and sends it to process j
//filedes[i][j][0] is for reading data and filedes[i][j][1] os for writing it

//This function converts this array into pipe
void initialise(){
    for(int i = 0; i < NO_OF_PROCESSES+1; ++i){
        for(int j = 0; j < NO_OF_PROCESSES+1; ++j){
            pipe(filedes[i][j]); //Creating a pipe
        }
    }
    return;
}

//This function can be used by a sender to  send a message
//sender: The id of the sender (The OS should ensure that sender must send its pid as the value in this field)
//receiver: The pid of the receiver 
//message: The value to be passed must be put in this parameter
void sendMessage(int sender, int receiver, int message){
    write(filedes[sender][receiver][1], &message, sizeof(int));
    //The above line writes the data in message into the pipe which sends it to the receiver
    return;
}

//This function reads the value present in the pipe
//sender: The message's source's pid (a process can be connected to several pipes, therefore the sender must be specified)
//receiver: The id of the receiver (The OS should ensure that the receiver must send its pid as the value in this field)
int readMessage(int sender, int receiver){
    int message; //Declaring the variable to receive the message from the pipe
    read(filedes[sender][receiver][0], &message, sizeof(int));
    //The above line reads the message in the pipe
    //In case no message is present in the pipe, the process is sent into wait state by the OS
    //The waiting process is awakened once a message is put inside the pipe by some sender
    return message;
}

#endif
