#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define n_pro 10
#define n_con 10
#define npc 2 // max items produced or consumed by a producer or consumer 

sem_t full;
pthread_mutex_t mutex;
  

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
    for(int i = 0; i < npc; i++) {
        item = rand()%100; // Produce an random item
        pthread_mutex_lock(&mutex);
        add(item);
        printf("Producer %d: Insert Item %d \n", *((int *)p),item);
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }
}

//consumer block code
void *consumer(void *c)
{   
    for(int i = 0; i < npc; i++) {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        int item = extract();
        printf("Consumer %d: Remove Item %d \n",*((int *)c),item);        
        pthread_mutex_unlock(&mutex);
    }
}


int main() {
    
    pthread_t pro[n_pro],con[n_con];
    pthread_mutex_init(&mutex, NULL);
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

    pthread_mutex_destroy(&mutex);
    sem_destroy(&full);

    return 0;
}
