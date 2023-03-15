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

sem_t full, empty, mutex;
int in=0, out=0;
int buffer[buffer_size];


void *producer(); 
void *consumer();

int main()
{
    pthread_t p[no_p], c[no_c];
    sem_init(&mutex, 0, 1);
    sem_init(&empty, 0, buffer_size);
    sem_init(&full, 0, 1);
    int max = MAX(no_p, no_c);
    int num[max];    
    
    for (int i=0; i<no_p; i++)
    {
        num[i] = i+1;
        pthread_create(&p[i], NULL, (void *)producer, (void *)&num[i]);
    }    
    for (int i=0; i<no_c; i++)
    {
        num[i] = i+1;
        pthread_create(&c[i], NULL, (void *)consumer, (void *)&num[i]);
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
        int nextProd = rand()%100;
        sem_wait(&empty);
        sem_wait(&mutex);
        buffer[in] = nextProd;
        printf("Producer %d: Insert Item %d \n", *((int *)p_no),buffer[in]);
        in = (in+1)%buffer_size;
        sem_post(&mutex);
        sem_post(&full);
    }

}

void *consumer(void *c_no)
{
    for (int i=0; i<max_cons; i++)
    {        
        sem_wait(&full);
        sem_wait(&mutex);
        int nextCons = buffer[out];        
        printf("Consumer %d: Remove Item %d \n",*((int *)c_no),nextCons);
        out = (out+1)%buffer_size;
        sem_post(&mutex);
        sem_post(&empty);
    }
}