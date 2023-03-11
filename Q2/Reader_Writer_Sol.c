#include <stdio.h>
#include <stdlib.h>
#include "semaphore.h"
#include <stdbool.h>

#define Readers 10 //No. of Readers
#define Writers 5  //No. of Writers 

sem_t in, out, write;

int data = 0; //shared data between reader and writer
//Reader will read this data and writer will increment it by 10
 
int reader_in=0, reader_out=0;
bool wrt_wait = false; //shows if a writer is waiting


void *Reader(void*);
void *Writer(void*);

int main() {
    pthread_t r[Readers], w[Writers];
    sem_init(&in, 0, 1);
    sem_init(&out, 0, 1);
    sem_init(&write, 0, 0);
    int rnum[Readers], wnum[Writers];
    

    for (int i=0; i<Writers; i++)
    {
        wnum[i] = i+1;
        pthread_create(&w[i], NULL, (void *)Writer, (void *)&wnum[i]);
    } 
    for (int i=0; i<Readers; i++)
    {
        rnum[i] = i+1;
        pthread_create(&r[i], NULL, (void *)Reader, (void *)&rnum[i]);
    }
       
    for (int i=0; i<Writers; i++)
    {
        pthread_join(w[i], NULL);
    } 
    for (int i=0; i<Readers; i++)
    {
        pthread_join(r[i], NULL);
    }
      

    sem_destroy(&in);
    sem_destroy(&out);
    sem_destroy(&write);
}

void *Reader(void *rno){
    sem_wait(&in);
    reader_in++;
    sem_post(&in);

    //Critical Section
    printf("Reader %d is reading value %d\n",*((int *)rno),data);

    sem_wait(&out);
    reader_out++;
    if (wrt_wait==1 && reader_in==reader_out)
        sem_post(&write);
    sem_post(&out);
}

void *Writer(void *wno){
    sem_wait(&in);
    sem_wait(&out);

    if (reader_in==reader_out)
        sem_post(&out);
    else {
        wrt_wait = true;
        sem_post(&out);
        sem_wait(&write);
        wrt_wait = false;
    }

    //Critical Section
    data +=10; //writer is increasing value of data by 10
    printf("Writer %d is writing value %d \n",*((int *)wno),data);

    sem_post(&in);
}
