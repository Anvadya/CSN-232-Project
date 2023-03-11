#include <stdio.h>
#include <stdlib.h>
#include "semaphore.h"

#define buffer_size 30
#define no_p 5 //No. of Producers
#define no_c 10 //No. of Consumers
#define max_prod 2 // Maximum items a producer can produce or a consumer can consume
#define max_cons (max_prod*no_p)/no_c // Maximum items a producer can produce or a consumer can consume

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

sem_t full, empty;
int buffer[buffer_size];
int top = 0;
pthread_mutex_t mutex;

void *producer(); 
void *consumer();

//Stack operations
void push(int i)
{
   if (top >= buffer_size)
   {
      return;
   }
   buffer[top] = i;
   top++;
}
int pop(void)
{
   top--;
   if (top < 0)
   {
      return 0;
   }
   return buffer[top];
}

int main()
{
    pthread_t p[no_p], c[no_c];
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, buffer_size);
    sem_init(&full, 0, 0);
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

    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);
}

void *producer(void *p_no) 
{
    for (int i=0; i<max_prod; i++)
    {
        int nextProd = rand()%100;
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        push(nextProd);
        printf("Producer %d: Insert Item %d\n", *((int *)p_no),nextProd);      
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }
}

void *consumer(void *c_no)
{
    for (int i=0; i<max_cons; i++)
    {        
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        int nextCons = pop();        
        printf("Consumer %d: Remove Item %d \n",*((int *)c_no),nextCons);
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
}