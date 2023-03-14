#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define n_pro 10
#define n_con 10
#define npc 2 // max items produced or consumed by a producer or consumer 
#define INITIAL_SIZE 8 //initial size of dynamic array

sem_t full;
int in = 0;
int out = 0;
pthread_mutex_t mutex;
  
// dynamic array defined
typedef struct {
    size_t size;
    size_t capacity;
    int* array;
}dynamic_array;

dynamic_array* buffer;

void arrayInit(dynamic_array** arr_ptr)
{
    dynamic_array *container;
    container = (dynamic_array*)malloc(sizeof(dynamic_array));
    if(!container) {
        printf("Memory Allocation Failed\n");
        exit(0);
    }
  
    container->size = 0;
    container->capacity = INITIAL_SIZE;
    container->array = (int *)malloc(INITIAL_SIZE * sizeof(int));
    if (!container->array){
        printf("Memory Allocation Failed\n");
        exit(0);
    }
  
    *arr_ptr = container;
}
  
void insertItem(dynamic_array* container, int item)
{
    if (container->size == container->capacity) {
        int *temp = container->array;
        container->capacity <<= 1;
        container->array = realloc(container->array, container->capacity * sizeof(int));
        if(!container->array) {
            printf("Out of Memory\n");
            container->array = temp;
            return;
        }
    }
    container->array[container->size++] = item;
}

int getItem(dynamic_array* container, int index)
{
    if(index >= container->size) {
        printf("Index Out of Bounds\n");
        return -1;
    }
    return container->array[index];
}

void freeArray(dynamic_array* container)
{
    free(container->array);
    free(container);
}


// producer block code
void *producer(void *p)
{   
    int item;
    for(int i = 0; i < npc; i++) {
        item = rand()%100; // Produce an random item
        pthread_mutex_lock(&mutex);
        insertItem(buffer, item);
        printf("Producer %d: Insert Item %d at %d\n", *((int *)p),getItem(buffer, in),in);
        in = (in+1);
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
        int item = getItem(buffer, out);
        printf("Consumer %d: Remove Item %d from %d\n",*((int *)c),item, out);
        out = (out+1);
        pthread_mutex_unlock(&mutex);
    }
}


int main() {
    arrayInit(&buffer);
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
    freeArray(buffer);

    return 0;
}
