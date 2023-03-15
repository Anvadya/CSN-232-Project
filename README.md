# CSN-232-Project

## Synchronization
Synchronization is the procedure involving the coordination of execution of multiple processes that share the same memory space in an operating system. A multi-process system may contain two or more process such that the execution is affected by or affects the execution of another process. Synchronization ensures that the order of the execution of these processes is preserved. It helps in maintaining `data consistency` and integrity, and avoiding the problems of `race condition`, `deadlocks` and other synchronization issues in a concurrent system. This is achieved using various techniques such as `semaphores`, `monitors`, and `critical sections`. Process synchronization constitute an essential part of the modern operating systems.

## Synchronization Primitves
1. `Semaphore` : Semaphore is a signalling mechanism. A thread that is waiting on a semaphore can be signalled by another thread. It uses two atomic operations, `wait` and `signal`, for proceess synchronization.
2. `Mutex` : Mutex is a locking system that synchronizes access to a resource shared by more than one process. 
3. `Pipe` : Pipe refers to a software connection between two processes, enabling one-way communication.

## Group Members 
1. Anvadya Khare (21114015)
2. Dheeraj Kumar Singh (21114034)
3. Anupriya Dey (21114014)
4. Rishika (21115117)
5. Garvit Goyal (21114036)
6. Komal Gupta (21114052)
7. Kulkarni Sourabh Shirinivasrao (21114053)
8. Shrey Gupta (21112103)
9. Tanmay Bakshi (21122048)
10. Tejas Sajwan (21114106)

## Problems 
* Q1: Producer–Consumer Problem (multiple producers, multiple consumers) 
  * Bounded buffer 
  * Cyclic buffer 
  * Infinite buffer 
* Q2: Readers Writers Problem (multiple readers, multiple writers) 
* Q3: Dining Philosophers Problem (multiple philosophers) 
* Q4: Sleeping Barbers Problem (multiple barbers, multiple customers, single queue) 
* Q5: Cigarette Smokers Problem (multiple smokers) 
* Q6: 
  * Dynamic Disk Allocation Problem
  * Lamport Clock Based Synchronization
