# Starve-Free Readers Writers Problem 
# Implementation with Multiple Readers and Multiple Writers

## Introduction:
The multiple reader, multiple writer problem is a classic synchronization problem in operating systems. It involves multiple processes accessing a shared resource concurrently, where some processes may read the resource and others may write to it. The main challenge is to ensure that no process is left waiting indefinitely, leading to starvation.

The solution to this problem using semaphores ensures that multiple readers can access the shared resource simultaneously, and writers can access the resource exclusively. Additionally, the solution ensures that no process is starved indefinitely.
## Solution

The essential idea is that a Writer informs Readers of their need to enter the working space. Also, no new Readers can begin working at this time. Once a reader starts executing it increments variable `reader_in`. Each Reader which leaves the working area increments variable `reader_out` and checks to see if a Writer is waiting. When the final Reader is to leave then `reader_in` will be equal to `reader_out` and alerts the Writer that it is safe to move further by giving semaphore `write`.

The writers wait on `in` and `out`. After acquiring both semaphores, the writes compare the value of `reader_in` and `reader_out`. If it is equal then no readers are executing in their critical section annd writer will continue to its critical section. If it is not equal then it waits for all readers processes to complete execution and turn `wrt_wait` to true to show that a writer process is waiting in queue. Once it gets `write` semaphore it proceeds to its critical section and changes `wrt_wait` to false. After access to the working area by writer is complete, the writer notifies any waiting readers that they are now able to access the working area once more.

## How to execute

```
git clone https://github.com/Anvadya/CSN-232-Project.git
cd CSN-232-Project/Q2
gcc Reader_Writer_Sol.c -o thread -lpthread
./thread
```
## Sample Output
```
Writer 1 is writing value 10 
Reader 2 is reading value 10
Writer 4 is writing value 20 
Reader 3 is reading value 20
Writer 3 is writing value 30 
Writer 5 is writing value 40 
Reader 5 is reading value 40
Writer 2 is writing value 50 
Reader 6 is reading value 50
Reader 1 is reading value 50
Reader 4 is reading value 50
Reader 8 is reading value 50
Reader 7 is reading value 50
Reader 9 is reading value 50
Reader 10 is reading value 50

```
## Conclusion

The solution presented here ensures that multiple readers can read the shared resource simultaneously, while writers have exclusive access to the resource. Additionally, the solution ensures that no process is starved indefinitely.
