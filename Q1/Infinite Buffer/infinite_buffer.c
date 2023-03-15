#include <stdio.h>
#include <pthread.h>
#include "semaphore.h"
#include <stdlib.h>
#include <pthread.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define n_pro 5
#define n_con 10
#define max_prod 2 // Maximum items a producer can produce or a consumer can consume
#define max_cons (max_prod*n_pro)/n_con // Maximum items a producer can produce or a consumer can consume 

sem_t full, mutex;

typedef struct queue queue;

struct queue{
    queue * back_pointer;
    int data;
};

queue * head = NULL;
queue * tail = NULL;

void add(int data){
    queue * tmp = (queue *) malloc(sizeof(queue));
    tmp->data = data;
    tmp->back_pointer = NULL;
    if(head == NULL){
        head = tail = tmp;
    }
    else{
        tail->back_pointer = tmp;
        tail = tmp;
    }
}

int extract(){
    if(head == NULL) return -1;
    queue *tmp = head;
    if(head->back_pointer == NULL){
        head = tail = NULL;
    }
    else{
        head = head->back_pointer;
    }
    int dataToBeReturned = tmp->data;
    free(tmp);
    return dataToBeReturned;
}

// producer block code
void *producer(void *p)
{   
    int item;
    for(int i = 0; i < max_prod; i++) {
        item = rand()%100; // Produce a random item
        sem_wait(&mutex);
        add(item);
        printf("Producer %d: Insert Item %d \n", *((int *)p),item);
        sem_post(&mutex);
        sem_post(&full);
    }
}

//consumer block code
void *consumer(void *c)
{   
    for(int i = 0; i < max_cons; i++) {
        sem_wait(&full);
        sem_wait(&mutex);
        int item = extract();
        printf("Consumer %d: Remove Item %d \n",*((int *)c),item);        
        sem_post(&mutex);
    }
}

int main() {
    
    pthread_t pro[n_pro],con[n_con];
    sem_init(&mutex, 0, 1);
    sem_init(&full,0,0);

    int max = MAX(n_pro, n_con);
    int num[max];

    for(int i = 0; i < n_pro; i++) {
        num[i] = i+1;
        pthread_create(&pro[i], NULL, (void *)producer, (void *)&num[i]);
    }
  
    for(int i = 0; i < n_con; i++) {
        num[i] = i+1;
        pthread_create(&con[i], NULL, (void *)consumer, (void *)&num[i]);
    }

    for(int i = 0; i < n_pro; i++) {
        pthread_join(pro[i], NULL);
    }
  
    for(int i = 0; i < n_con; i++) {
        pthread_join(con[i], NULL);
    }

    sem_destroy(&mutex);
    sem_destroy(&full);

    return 0;
}
