#include <stdio.h>
#include <stdlib.h>
#include "semaphore.h"
#include <stdbool.h>
#include <pthread.h>

#define Readers 10 //No. of Readers
#define Writers 5  //No. of Writers 

sem_t in, out, writer; //Semaphores declared



int data = 0; //shared data between reader and writer
//Reader will read this data and writer will increment it by 10
 
int reader_in=0, reader_out=0; 
//reader_in is the number of readers that have started reading
//reader_out is the number of readers that have completed reading
bool wrt_wait = false; //shows if a writer is waiting

void *Reader(void*); //Reader function is declared
void *Writer(void*); //Writer function is declared

int main() {
    pthread_t r[Readers], w[Writers]; //initialising reader and writer threads
    sem_init(&in, 0, 1); //initialising semaphore in with value 1
    sem_init(&out, 0, 1); //initialising semaphore out with value 1
    sem_init(&writer, 0, 0); //initialising semaphore write with value 0
    int rnum[Readers], wnum[Writers];
    

    for (int i=0; i<Writers; i++)
    {
        //Writer threads created
        wnum[i] = i+1;
        pthread_create(&w[i], NULL, (void *)Writer, (void *)&wnum[i]); 
    } 
    for (int i=0; i<Readers; i++)
    {
        //Reader threads created
        rnum[i] = i+1;
        pthread_create(&r[i], NULL, (void *)Reader, (void *)&rnum[i]); 
    }
       
    for (int i=0; i<Writers; i++)
    {
        //Merging threads (to avoid zombie threads)
        pthread_join(w[i], NULL);
    } 
    for (int i=0; i<Readers; i++)
    {
        //Merging threads (to avoid zombie threads)
        pthread_join(r[i], NULL);
    }

    //Semaphores are destroyed
    sem_destroy(&in);
    sem_destroy(&out);
    sem_destroy(&writer);
}


//Reader function
void *Reader(void *rno){    
    sem_wait(&in); //Reader waits on semaphore 'in'
    //Once it acquires semaphore 'in' reader starts executing and enters C.S.
    reader_in++; //Next Reader arrives
    sem_post(&in); 

    //Critical Section
    printf("Reader %d is reading value %d\n",*((int *)rno),data);

    sem_wait(&out);
    //On acquiring semaphore 'out' it exits C.S.
    reader_out++; //Reader completes reading

    //if all readers finish reading and writer is waiting (wrt_wait=1) then writer will aquire the write semaphore
    if (wrt_wait==1 && reader_in==reader_out)
        sem_post(&writer);
    sem_post(&out);
}

//Writer Function
void *Writer(void *wno){
    sem_wait(&in); //writer waits on semaphore 'in' 
    sem_wait(&out); //writer waits on semaphore 'out'
    //Once it acquires semaphore 'in' and 'out' writer starts executing and enters C.S.

    if (reader_in==reader_out) //checks if all readers have finished reading
        sem_post(&out);
    else {
        //if all readers have not finished their execution then 'wrt_wait will be set to true to show that a writer is waiting
        wrt_wait = true; 
        sem_post(&out);
        //writer will wait on write semaphore which it will get once all readers have finished their execution.
        sem_wait(&writer);
        //after getting write semaphore, writer proceeds to CS and wrt_wait is set to false
        wrt_wait = false;
    }

    //Critical Section
    data +=10; //writer is increasing value of data by 10
    printf("Writer %d is writing value %d \n",*((int *)wno),data);

    //writer has finished its execution of CS.
    sem_post(&in);
}
