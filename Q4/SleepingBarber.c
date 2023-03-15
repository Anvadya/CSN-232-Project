// Include standard libraries
#include <unistd.h>          
#include <stdio.h>           
#include <stdlib.h>          
#include <pthread.h>  

// Include the header file
#include "semaphore.h"

// Define various constants
#define TOTAL_CHAIRS 16         // Total capacity of the waiting room
#define BARBER_COUNT 4          // Total number of barbers
#define CUSTOMER_COUNT 30       // Total number of customers
#define TIME 3                  // Hair cutting time for each customer
#define UPPER 800000            // Vary these UPPER and LOWER values to vary the time gap in between customers arrival
#define LOWER 200000

// Variable initialization

// Semaphores
sem_t customers;                 
sem_t barbers;                   
sem_t mutex;                     

// Other Variables
int freeSeats = TOTAL_CHAIRS;           
int seatPocket[TOTAL_CHAIRS];           // To exchange pid between customer and barber
int nextSeat = 0;                       // Index for next available seat
int next = 0;                           // Index for choosing customer whose hair is to be cut
static int count = 0;                   // Counter of customers

// Function declaration
void barberThread(void *tmp);           // Thread Function for barber
void customerThread(void *tmp);         // Thread Function for customer
void delay();                           // Randomized delay function

// Main function 
int main()
{   
    // pthread_t is the data type used to uniquely identify a thread. In a vague sense it is thread ID.
    pthread_t customer[CUSTOMER_COUNT] ,barber[BARBER_COUNT]; // Declared an array of threads

    //Semaphore initialization
    sem_init(&customers,0,0);
    sem_init(&barbers,0,0);
    sem_init(&mutex,0,1);   

    printf("Execution Started.\n");                     

    //Barber thread initialization
    for(int i=0;i<BARBER_COUNT;i++)                     // Creation of Barber Threads
    {   
       int status=pthread_create(&barber[i],NULL,(void *)barberThread,(void*)&i);
       sleep(1);
    }

    //Customer thread initialization
    for(int i=0;i<CUSTOMER_COUNT;i++)                   // Creation of Customer Threads
    {   
       int status=pthread_create(&customer[i],NULL,(void *)customerThread,(void*)&i);
       delay();                                         // Customers arrive at random intervals
    }   

    for(int i=0;i<CUSTOMER_COUNT;i++)                   // Waiting till all customers are dealt with
    {
        pthread_join(customer[i],NULL);
    }

    sleep(2*TIME);                                      // It is required so that all barbers are done with their jobs before execution ends
    printf("Execution Completed.\n");
    exit(EXIT_SUCCESS);                                 // Exit abandoning infinite loop of barber thread
}

// Function to create delay
void delay()
{
    int x = (rand() % (UPPER - LOWER + 1)) + LOWER; 
    srand(time(NULL));
    usleep(x);          // execution stopped for x microseconds
}

//Customer process
void customerThread(void *tmp) 
{   
    int seat;
    sem_wait(&mutex);                                   // Mutex to protect seat changes
    count++;                                            
    printf("Customer-%d with Id:%d has entered the shop. \n",count,pthread_self());
    if(freeSeats > 0) 
    {
        freeSeats--;                                    
        printf("Customer-%d is sitting on a chair in the waiting room.\n",count);
        nextSeat = (++nextSeat) % TOTAL_CHAIRS;         // Select a chair to sit
        seat = nextSeat;
        seatPocket[seat] = count;                       // Tells who is sitting on the seat
        sem_post(&mutex);                               // Release the mutex
        sem_post(&barbers);                             // Wake up barber
        sem_wait(&customers);                           // Add to customers queue
        sem_wait(&mutex);                               // Mutex to protect seat changes                       
        freeSeats++;                            
        sem_post(&mutex);                               // Release the mutex
    } 
    else 
    {
       sem_post(&mutex);                                // Customer leaves without haircut
       printf("Customer-%d can't get a seat and has left the shop.\n",count);
    }
    pthread_exit(0);
}

// Barber Process
void barberThread(void *tmp) 
{   
    int index = *(int *)(tmp);      
    int myNext, c;
    printf("Barber-%d with Id:%d has entered the Shop. \n",index,pthread_self());
    while(1) 
    {   
        printf("Barber-%d has gone to sleep.\n",index);
        sem_wait(&barbers);                                 // Barber goes to sleep
        sem_wait(&mutex);                                   // Mutex to protect seat changes
        next++;
        next = (next) % TOTAL_CHAIRS;                       // Select next customer
        c = seatPocket[next];                  
        seatPocket[next] =(pthread_self());     
        sem_post(&mutex);
        sem_post(&customers);                               //Customer 'c' is getting a haircut
        printf("Barber-%d was woken up by Customer-%d and is cutting his/her hair.\n",index,c);
        sleep(TIME);
        printf("Barber-%d has finished cutting hair. \n",index);
    }
}
