#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

#define n_pro 10
#define n_con 5

sem_t empty;
sem_t full;
int in = 0;
int out = 0;
int buffer[1000005]; //have to create a dynamic array
pthread_mutex_t mutex;

void *producer(void *p)
{   
    int item;
    for(int i = 0; i < 1000005; i++) {
        item = rand(); // Produce an random item
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        buffer[in] = item;
        printf("Producer %d: Insert Item %d at %d\n", *((int *)p),buffer[in],in);
        in = (in+1);
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }
}

void *consumer(void *c)
{   
    for(int i = 0; i < 1000005; i++) {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        int item = buffer[out];
        printf("Consumer %d: Remove Item %d from %d\n",*((int *)c),item, out);
        out = (out+1);
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
}


int main() {
    pthread_t pro[n_pro],con[6];
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty,0,1000005);
    sem_init(&full,0,0);

    int a[6] = {1,2,3,4,5,6}; //Just used for numbering the producer and consumer

    for(int i = 0; i < n_pro; i++) {
        pthread_create(&pro[i], NULL, (void *)producer, (void *)&a[i]);
    }
  
    for(int i = 0; i < n_con; i++) {
        pthread_create(&con[i], NULL, (void *)consumer, (void *)&a[i]);
    }

    for(int i = 0; i < n_pro; i++) {
        pthread_join(pro[i], NULL);
    }
  
    for(int i = 0; i < n_con; i++) {
        pthread_join(con[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
    
}

