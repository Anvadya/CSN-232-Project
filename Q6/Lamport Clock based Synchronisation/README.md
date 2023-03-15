# LAMPORT'S CLOCK SYNCHRONIZATION

Lamport timestamp algorithm is one of the simplest algorithms to maintain the order of events in a distributed system.

A distributed system consists of a collection of distinct processes which are spatially seperated, 
and which communicate with one another by exchanging messages.
In a distributed system, it is sometimes impossible to say that one of two events occured first. 

Thus, the relation "happened before" may only provide a partial ordering of the events occuring in a distributed system.
This might lead to problems in terms of functionality of the system. 

Consider the following definition of a system - Assume a set of processes running concurrently. Each process consists of a sequence of events/operations.
Depending on the application, this operation could be a single machine instruction, or even the execution of a subroutine.

---
For simplicity, we are assuming equal number of operations in each process - 

```C
#define NO_OF_OPERATIONS 8 
```

In a distributed system, we can define 3 types of events that each process can execute.

1. A local event
2. A sent event 
3. A receive event

We now introduce the concept of clocks/timestamps - Think of them as a way of assigning a number to an event,
where the number can be thought of as the time at which the event occured.

The events update the logical timestamp according to the below rules : 

> 1.) Before executing an event, the process increments the logical timestamp by 1.

```C
Logical_timestamp = Logical_timestamp + 1
```

> 2.) During a send event, the sender process increments the logical timestamp by 1 and sends the time along with the message.

```C
Logical_timestamp = Logical_timestamp + 1 
```

> 3.) During a receive event, the counter of the reciepient is updated to the max value of its own, local timestamp; and the timestamp present in the received message.
> This timestamp is then incremented by 1.

```C
Logical_timestamp = max(Logical_timestamp, time_received);
Logical_timestamp++;
```

Note - 
> For our implementation, we consider each timestamp as an operation, i.e., our timestamp unit is
> such that each unit increment represents the next operation along the causal line.

---

The following data structures are of importance - 

```C
int operations[NO_OF_PROCESSES+1][NO_OF_OPERATIONS];
// A matrix that maintains send/receive transactions among the operations.
/*
----- */
for(int i = 0; i < NO_OF_PROCESSES+1; ++i){
        for(int j = 0; j < NO_OF_OPERATIONS; ++j){
            operations[i][j] = i;
        }
    }
//We initialise the matrix such that the entry corresponding to an operations contains the parent process number.
//In a way, default setting maps each operation to its parent causal line.

//We now define the following send/receive transactions - 
operations[1][4] = 2;  //Process(1) sends, at timestamp=4, a message to Process(2)
operations[2][2] = -1; //Process(2) receives, at timestamp=2, a message from Process(1)
operations[2][4] = 1; //Process(2) sends, at timestamp=4, a message to Process(1)
operations[1][6] = -2; //Process(1) receive, at timestamp=6, a message from Process(2)
```
---

```C
int pid[NO_OF_PROCESSES+1];
/*
.
. */
for(int i = 0; i < NO_OF_PROCESSES+1; ++i) pid[i] = i;
```
The array `pid[]` stores the assigned process ID for each of the processes (as a serial number in this case.)

---

```C
pthread_t process_thr[NO_OF_PROCESSES];
```
`pthread_t` is the data-type used to define a thread object.
We define an array to store thread objects, with each object intended to run a separate process.

***

The subroutine `process()` is run by each thread. The process ID, i.e. `pid[i+1]` is passed as its argument for each subsequent thread.
```C

int main() {
/*
------
*/
    for(int i = 0; i < NO_OF_PROCESSES; ++i){
        pthread_create(&process_thr[i], NULL, (void *)process, (void *)&pid[i+1]);
    }
    for(int i = 0; i < NO_OF_PROCESSES; ++i){
        pthread_join(process_thr[i], NULL);
    }
}

//process_thr[i] is returned by pthread_create() and used by the application in functional calls that require a thread identifier.
```

> Here, the `pthread_create()` function starts a new thread in the calling process. 
> The new thread starts execution by invoking `(void *)process`; and a reference to `pid[i+1]` is passed as the sole argument to `process`.

Thus, threads equal to the number of processes are created here and they start executing.

> `pthread_join()` function waits for the thread specified by `process_thr[i]` to terminate.
> If that thread has already terminated, then it returns immediately.

---

### THREADED IMPLEMENTATION

Thus, the processes are assigned seperate, newly created threads to run, and inter-thread communication required to provide partial ordering is facilitated through the usage of pipes.

Note - 
> Using pipes to implement the distributed Lamport clock ensures that the events are ordered correctly, 
> and the communication between the processes is synchronized and reliable. However, it is important to 
> note that using pipes for inter-process communication can introduce performance overheads and scalability 
> issues, and other mechanisms such as sockets or message queues may be more appropriate in certain scenarios.

---

### EXPECTED OUTPUT : 

```C
Clock for process: 1 is 1
Clock for process: 1 is 2
Clock for process: 1 is 3
Clock for process: 1 is 4
Writing data from 1 to 2 at time 4
Clock for process: 1 is 5
Clock for process: 1 is 6
Clock for process: 2 is 1
Clock for process: 2 is 2
Read data from 1 by 2, data = 1
Read clock time from 1 by 2, clock time = 4
Clock for process: 2 is 6
Clock for process: 2 is 7
Writing data from 2 to 1 at time 7
Clock for process: 2 is 8
Clock for process: 2 is 9
Clock for process: 2 is 10
Clock for process: 2 is 11
Read data from 2 by 1, data = 2
Read clock time from 2 by 1, clock time = 7
Clock for process: 1 is 9
Clock for process: 1 is 10
[1] + Done                       "/usr/bin/gdb" --interpreter=mi --tty=${DbgTerm} 0<"/tmp/Microsoft-MIEngine-In-5q54oaby.1gf" 1>"/tmp/Microsoft-MIEngine-Out-iwz45542.otf"
```
