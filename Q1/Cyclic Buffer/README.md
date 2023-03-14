# Starve-Free; Deadlock-Free Producer-Consumer Problem
# Implementation using multiple producer and multiple consumer

## Problem Statement
The multiple producer, multiple consumer problem is a classic synchronization
problem in operating systems.This file explains the implementation 
of the starve-free multiple producer-consumer problem using a cyclic
buffer. The goal is to create a system where multiple producers can add items
to the buffer and multiple consumers can remove items from the buffer without
any starvation issues.

## Solution
The implementation uses two types of threads, one for producers and one for 
consumers. Each thread performs its respective function, either adding or 
removing items from the buffer. In the prosposed starve free solution, I have
used two semaphores for denoting full and empty buffer and a mutex for serving
the purpose of mutual exclusion.
A fixed-size buffer array is used which stores item number produced by the producer
and the 'in' variable is accordingly incremented. Whenever a consumer comes, it'll 
consume the item present at the 'out' index and it's then incremented. This makes it
a First-in-first-out (FIFO) implementation.
When the index reaches the end of the buffer, it wraps around to the beginning. 

## How to use
```
Clone the repository: git clone https://github.com/Anvadya/CSN-232-Project.git
Navigate to the directory: cd CSN-232-Project/Q1/Cyclic\ Buffer/
Compile the code: gcc Producer_Consumer_Cyclic.c -o thread -lpthread
Run the program: ./thread
```

## Sample Output
```
Producer 1: Insert Item 83 
Producer 1: Insert Item 93 
Producer 3: Insert Item 77 
Producer 3: Insert Item 35 
Producer 5: Insert Item 86 
Producer 5: Insert Item 92 
Consumer 2: Remove Item 83 
Producer 4: Insert Item 15 
Producer 4: Insert Item 49 
Producer 2: Insert Item 86 
Producer 2: Insert Item 21 
Consumer 3: Remove Item 93 
Consumer 1: Remove Item 77 
Consumer 4: Remove Item 35 
Consumer 5: Remove Item 86 
Consumer 6: Remove Item 92 
Consumer 7: Remove Item 15 
Consumer 8: Remove Item 49 
Consumer 9: Remove Item 86 
Consumer 10: Remove Item 21
```

## Conclusion
This implementation provides a starve-free solution to the multiple
producer multiple consumer problem using a cyclic buffer. The use of
semaphores and threads ensures synchronization between producers and 
consumers and avoids race conditions. This implementation can be used 
in various scenarios where multiple producers and consumers need to access
a shared resource without any starvation issues.

