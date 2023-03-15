#include <stdio.h>
#include <stdlib.h>
#include "semaphore.h"
#include <pthread.h>

#define buffer_size 30
#define no_p 5 //No. of Producers
#define no_c 10 //No. of Consumers
#define max_prod 3 // Maximum items a producer can produce or a consumer can consume
#define max_cons (max_prod*no_p)/no_c // Maximum items a producer can produce or a consumer can consume

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

sem_t full, empty, mutex;  //Created three semaphores denoting full and empty buffers, mutex
int in=0, out=0;   //index of buffer array to indicate where an item is produced and from where it's consumed
int buffer[buffer_size];  //buffer array containing items produced by producer


void *producer(); 
void *consumer();

int main()
{
    pthread_t p[no_p], c[no_c];   //defining two pthreads for producer and consumer
    sem_init(&mutex, 0, 1); //initialising mutex
    sem_init(&empty, 0, buffer_size);   //initialising empty semaphore as size of buffer
    sem_init(&full, 0, 1);    //initialising full semaphore as 0
    int max = MAX(no_p, no_c);
    int num[max];      //array representing no. of producers and consumers
    
    for (int i=0; i<no_p; i++)
    {
        num[i] = i+1;
        pthread_create(&p[i], NULL, (void *)producer, (void *)&num[i]);   //creating pthread for producer
    }    
    for (int i=0; i<no_c; i++)
    {
        num[i] = i+1;
        pthread_create(&c[i], NULL, (void *)consumer, (void *)&num[i]);     //creating pthread for consumer
    }
    
    for (int i=0; i<no_p; i++)
    {
        pthread_join(p[i], NULL);
    }
    for (int i=0; i<no_c; i++)
    {
        pthread_join(c[i], NULL);
    }

    sem_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);
}

void *producer(void *p_no) 
{
    for (int i=0; i<max_prod; i++)
    {
        int nextProd = rand()%100;    //item no. is generated using random generator
        sem_wait(&empty);              //producer will have to wait if buffer is full i.e. empty is 0
        sem_wait(&mutex);
        buffer[in] = nextProd;       //item no. is then inserted in the buffer array
        printf("Producer %d: Insert Item %d \n", *((int *)p_no),buffer[in]);
        in = (in+1)%buffer_size;     //represents the cyclic flow of index
        sem_post(&mutex);
        sem_post(&full);          //semaphore full will be incremented once an item is produced and inserted to array
    }

}

void *consumer(void *c_no)
{
    for (int i=0; i<max_cons; i++)
    {        
        sem_wait(&full);      //consumer will have to wait if the buffer is empty i.e. full is 0
        sem_wait(&mutex);
        int nextCons = buffer[out];     //consumer will consume the initially produced item and proceeds accordingly
        printf("Consumer %d: Remove Item %d \n",*((int *)c_no),nextCons);
        out = (out+1)%buffer_size;     //represents the cyclic flow of index
        sem_post(&mutex);
        sem_post(&empty);       //semaphore empty will be incremented once an item is consumed as one item is removed
    }
}
