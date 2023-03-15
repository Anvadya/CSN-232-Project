//                                           CSN-232 GROUP PRJECT
//                                         DINING PHILOSOPHERS PROBLEM
//                           Solved by  :KULKARNI SOURABH SHRINIVASRAO(21114053)

// Avoiding deadlock: In this implementation to avoid deadlock, we employ assymetric implementation in which odd numbered
// philosopher will first get the right chopstick and even numbered will get the left one first.this will avoid the deadlock

// Avoiding starvation: for this, we maintain an array count[N] which stores how many times ith philosopher has eaten. we allow
//  ith philosopher to eat only if he has eaten less than his neighbours.In this way, we make both chopsticks available to the philosopher who Thus, no philosopher can get starved for long periods.

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "semaphore.h" //IMPLEMENTED BY GROUP MEMBERS, NOT THE STANDARD ONE.
#define N 7            // can be varied
void *liftTheLeftChopstick(int n);
void *liftTheRightChopstick(int n);
pthread_t philosopher[N];     // N philosophers implemented as N parallel threads.
sem_t chopstick[N]; // N chopsticks as N mutex locks
int count[N];

int main()
{
    for (int i = 0; i < N; i++)
    {
        count[i] = 0;
    }
    int i, k; // i for iteration purposes.k for storing return values of different pthread functions
    void *msg;
    for (i = 0; i < N; i++)
    {
        sem_init(&chopstick[i],0,1); // creating mutex
    }
    for (i = 0; i < N; i++) // creating a thread for each philosopher
    {
        // TO AVOID DEADLOCK, we implement the asymmetrcial solution.
        if (i % 2 == 0)
        {
            // if id of philosopher is even, he picks LEFT chopstick FIRST  and then Right one.
            k = pthread_create(&philosopher[i], NULL, (void *)liftTheLeftChopstick, (int *)i);
        }
        else
        {
            // if id of philosopher is ODD, he picks RIGHT chopstick FIRST  and then Left one.
            k = pthread_create(&philosopher[i], NULL, (void *)liftTheRightChopstick, (int *)i);
        }
    }
    for (i = 0; i < N; i++)
    { // for joining all threads.(i.e. to check if all threads are terminated successfully)
        k = pthread_join(philosopher[i], &msg);
        if (k != 0)
        {
            printf("\n Thread join failed \n");
            exit(1);
        }
    }
    for (i = 0; i < N; i++)
    {
        sem_destroy(&chopstick[i]); // destryoing mutex as work is done
        
    }
    return 0;
}

// function for eating, thinking for philosophers with even indices
void *liftTheLeftChopstick(int n)
{
    while (1)
    {   // we don't know how much times a philosopher will eat so we must have a infinite loop

        int state = rand() % 2; // As given in standard problem statement, philosopher may wish to  eat/think randomly

        if (state == 1)
        { // philosopher is hungry
            // when philosopher i is eating, he takes fork i and fork (i+1)%N
            if (count[n] <= count[(n + N - 1) % N] && count[n] <= count[(n + 1) % N])
            {
                // we allow  philosopher to eat only if he has eaten less or equal to than his neighbours
                //acquire the semaphores
               sem_wait(&chopstick[n]);
               sem_wait(&chopstick[(n + 1) % N]);

                printf("\nPhilosopher %d is eating ", n);
                count[n]++;
                sleep(rand() % N + 1); // random time 1-N can be needed for eating
                //relase the semaphores
                sem_post(&chopstick[n]);
                sem_post(&chopstick[(n + 1) % N]);
                printf("\nPhilosopher %d Finished eating ", n);
            }
            else
            {
                printf("\nPhilosopher %d didn't eat to avoid starvation ", n);
            }
        }
        else
        { // thinking is done
            printf("\nPhilosopher %d is thinking ", n);
            sleep(2);
        }
        sleep(5); // to avoid jumbling at console
    }
}

// function for eating, thinking for philosophers with odd indices

void *liftTheRightChopstick(int n)
{ // the rest procedure is exactly same as liftTheLeftChopstick(), just sequence of lifting chopsticks is reversed
    while (1)
    {

        int state = rand() % 2;
        state = 1;

        if (state == 1)
        {
            if (count[n] <= count[(n + N - 1) % N] && count[n] <= count[(n + 1) % N])
            {
                // sequence of lifting chopsticks is reversed
               sem_wait(&chopstick[(n + 1) % N]);
                sem_wait(&chopstick[n]);

                printf("\nPhilosopher %d is eating ", n);
                count[n]++;
                sleep(rand() % N + 1);
                sem_post(&chopstick[(n + 1) % N]);
                sem_post(&chopstick[n]);
                printf("\nPhilosopher %d Finished eating ", n);
            }
            else
            {
                printf("\nPhilosopher %d didn't eat to avoid starvation ", n);
            }
        }
        else
        {
            printf("\nPhilosopher %d is thinking ", n);
            sleep(2);
        }
        sleep(5);
    }
}
// to execute:
// gcc -pthread -o h h.c
//./c
