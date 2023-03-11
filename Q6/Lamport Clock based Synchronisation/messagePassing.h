#ifndef MESSAGEP_H_INCLUDED
#define MESSAGEP_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

#define NO_OF_PROCESSES 2

int filedes[NO_OF_PROCESSES+1][NO_OF_PROCESSES+1][2];

void initialise(){
    for(int i = 0; i < NO_OF_PROCESSES+1; ++i){
        for(int j = 0; j < NO_OF_PROCESSES+1; ++j){
            pipe(filedes[i][j]);
        }
    }
    return;
}

void sendMessage(int sender, int receiver, int message){
    write(filedes[sender][receiver][1], &message, sizeof(int));
    return;
}

int readMessage(int sender, int receiver){
    int message;
    read(filedes[sender][receiver][0], &message, sizeof(int));
    return message;
}

#endif