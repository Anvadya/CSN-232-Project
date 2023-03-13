#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "semaphore.h"
#include <unistd.h>
#include <array>
#include <iostream>

using namespace std;

sem_t sem_for_smokers[3]; //sem_for_smokers[i] -> semaphore for ith smoker
sem_t sem_for_agents; // semaphore for agent
int global=-1;

int cigarattes_smoked[3]; // cigarattes_smoked[i] -> number of ciggarates remaining to be smoked by ith smoker  

const int cigarattes_to_be_smoked_initially=2; // value of each cigarattes_smoked[i] initially

bool smoking_done=false; //Used to determine if all the smokers are done smoking
sem_t agent_is_ready; //Used to indicate when the agent is ready to complete his next turn
sem_t smokerReady; //Used to indicate the agent that the smoker is waiting for the agent to throw the items

int s=1;

string agents_provide[]={"matches & paper"," tobacco & paper ","matches and tobacco"};

void *smoker(void *pVoid)
{
    int *sem_number= (int*)(pVoid);

    // i indicates the number of times per thread smoking will be done
    for (int i = 0; i <cigarattes_to_be_smoked_initially; ++i)
    {
        usleep( rand() % 50000 );
        cout<<"Smoker "<<*sem_number<<" waiting for "<<agents_provide[*sem_number-1]<<" "<<endl;
        s++;
        if ( s>3 )
        sem_post(&smokerReady);

        sem_wait(&sem_for_smokers[(*sem_number-1)]);
        if ( global == 0 )// for smoker 1
        {

            usleep( rand() % 50000 );
            cout<<"Smoker1 is making a cigarette"<<endl;
	    sleep(01);
	    cout<<"Now Smoking\n";
	
            cigarattes_smoked[0]--;
            sem_post(&sem_for_agents);

        }

        else  if ( global == 1 )// for smoker 2
        {

            usleep(rand()%50000);
            cout<<"Smoker2 is making a cigarette"<<endl;
		sleep(1);
	    cout<<"Now Smoking\n";
	    cigarattes_smoked[1]--;
            sem_post(&sem_for_agents);
        }
        else if ( global == 2 )// for smoker 3 
        {

            usleep(rand()%50000);
            cout<<"Smoker3 is making a cigarette"<<endl;
		sleep(1);
	    cout<<"Now Smoking\n";
	    cigarattes_smoked[2]--;
            sem_post(&sem_for_agents);
        }
    }
		
    pthread_exit(NULL);
}

void *agent(void *pVoid)
{

    srand(time(NULL));// seeding for the rand function 
    int *sem_number= (int*)(pVoid);
    sem_wait(&agent_is_ready);
    for (int i = 0; i < 3*cigarattes_to_be_smoked_initially; ++i)
    {
	sem_wait(&sem_for_agents);

        while(true)
        {

            int random_integer=(rand()%3);
            usleep(rand()%4000);
	    if( cigarattes_smoked[0]==0 && cigarattes_smoked[1]==0 && cigarattes_smoked[2]==0 )
            {
                smoking_done = true;
                break;
            }
            else if(cigarattes_smoked[random_integer]!=0)
            {	sleep(1);
		cout<<"\nAgent throw"<<agents_provide[random_integer]<<endl;
                global=random_integer;
                sem_post(&sem_for_smokers[random_integer]);
                break;

            }	
	}
    }

}
void * func(void *pVoid)
{

    sem_wait( &smokerReady );

    sem_post( &agent_is_ready );

}

int main(void)
{
    for (int i = 0; i < 3; ++i)
    {
        cigarattes_smoked[i]=cigarattes_to_be_smoked_initially;
    }
	//initially assigning smoker 1 tobbaco
	//initially assigning smoker 2 matches
	//initially assigning smoker 3 paper
    sem_init(&sem_for_smokers[0],0,0); // it is initialized to 0 
    sem_init(&sem_for_smokers[1],0,0); // it is initialized to 0
    sem_init(&sem_for_smokers[2],0,0); // it is initialized to 0
	
	//initialisation of sem_for_agent
    sem_init(&sem_for_agents,0,1); // it is initialized to 1 
	
    //Indicating Semaphores
    sem_init(&agent_is_ready,0,0); // it is initialized to 0
    sem_init(&smokerReady,0,0); // it is initialized to 0
   
    pthread_t smoker1,smoker2,smoker3,agent1,pusher;
	
    int * smoker_number_1 = new int[1];
    smoker_number_1[0]=1;

    int * smoker_number_2 = new int[1];
    smoker_number_2[0]=2;

    int * smoker_number_3 = new int[1];
    smoker_number_3[0]=3;

    pthread_create(&smoker1, NULL, smoker,smoker_number_1 );
	
    pthread_create(&smoker2,NULL,smoker,smoker_number_2);
    	
    pthread_create(&smoker3,NULL,smoker,smoker_number_3);

    pthread_create(&agent1,NULL,agent,smoker_number_1);

    pthread_create(&pusher,NULL,func,NULL);

    pthread_join(smoker1,NULL);
    pthread_join(smoker2,NULL);
    pthread_join(smoker3,NULL);
	
    return 0;
}
