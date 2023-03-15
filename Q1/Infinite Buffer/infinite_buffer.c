#include <stdio.h>
#include <pthread.h>
#include "semaphore.h"
#include <stdlib.h>
#include <pthread.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define n_pro 5 //No. of Producers
#define n_con 10 //No. of Consumers
#define max_prod 2 // Maximum items a producer can produce or a consumer can consume
#define max_cons (max_prod*n_pro)/n_con // Maximum items a producer can produce or a consumer can consume 

sem_t full, mutex; //Created two semaphores denoting full buffer, mutex

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
        item = rand()%100; //item no. is generated using random generator
        sem_wait(&mutex);
        add(item); //item no. is then inserted in the buffer queue
        printf("Producer %d: Insert Item %d \n", *((int *)p),item);
        sem_post(&mutex);
        sem_post(&full); //semaphore full will be incremented once an item is produced and inserted to queue
    }
}

//consumer block code
void *consumer(void *c)
{   
    for(int i = 0; i < max_cons; i++) {
        sem_wait(&full); //consumer will have to wait if the buffer is empty i.e. full is 0
        sem_wait(&mutex);
        int item = extract();  //consumer will consume the initially produced item and proceeds accordingly
        printf("Consumer %d: Remove Item %d \n",*((int *)c),item);        
        sem_post(&mutex); 
    }
}

int main() {
    
    pthread_t pro[n_pro],con[n_con];  //defining two pthreads for producer and consumer
    sem_init(&mutex, 0, 1); //initialising mutex
    sem_init(&full,0,0); //initialising full semaphore as 0

    int max = MAX(n_pro, n_con); 
    int num[max]; //array representing no. of producers and consumers

    for(int i = 0; i < n_pro; i++) {
        num[i] = i+1;
        pthread_create(&pro[i], NULL, (void *)producer, (void *)&num[i]); //creating pthread for producer
    }
  
    for(int i = 0; i < n_con; i++) {
        num[i] = i+1;
        pthread_create(&con[i], NULL, (void *)consumer, (void *)&num[i]); //creating pthread for consumer
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
