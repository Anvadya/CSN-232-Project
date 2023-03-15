 # CSN-232 GROUP PROJECT
 ##    DINING PHILOSOPHERS PROBLEM
 ### Solved by: KULKARNI SOURABH SHRINIVASRAO(21114053)

 ### Features: **Deadlock free, starvation free, variable number of philosophers.**

The dining philosopher's problem is the classical problem of synchronization which says that Five philosophers are sitting around a circular table and their job is to think and eat alternatively. A bowl of noodles is placed at the center of the table along with five chopsticks for each of the philosophers. To eat a philosopher needs both their right and a left chopstick. A philosopher can only eat if both immediate left and right chopsticks of the philosopher is available. In case if both immediate left and right chopsticks of the philosopher are not available then the philosopher puts down their (either left or right) chopstick and starts thinking again.
## Solution:
 The synchronization problem can be solved using help of semaphores. Each philosopher tries to eat only after he has access to both the chopsticks. This can be implemented by treating each philosopher as an independant thread of execution and each chopstick as a semaphore.Just waiting for chopsticks and eating can lead to starvation( a philosopher is not able to eat even though he is hungry) and deadlocks(a condition where no philosopher can eat as everyone is waiting for other's chopsticks.).

### Avoiding deadlock:
  In this implementation to avoid deadlock, we employ assymetric implementation in which odd numbered philosopher will first get the right chopstick and even numbered will get the left one first.this will avoid the deadlock.That is, even numbered philosopher will be executing under `void *liftTheLeftChopstick(int n)` function and odd numbered will execute under `void *liftTheRightChopstick(int n)`.

### Avoiding starvation:
 For this, we maintain an array `count[N]` which stores how many times ith philosopher has eaten. we allow a philosopher to eat only if he has eaten less than his neighbours.In this way, we make both chopsticks available to the philosopher who is starving for duration greater than its neighbours.Thus, no philosopher can get starved for long periods.

 ### expected output:
   ```
   .
   .
   .
   Philosopher 4 is thinking
Philosopher 6 is thinking 
Philosopher 2 Finished eating
Philosopher 5 didn't eat to avoid starvation
Philosopher 1 is eating 
.
.
.
```

### For executing:
execute following commands in a linux system:
```
  gcc -pthread -o h <fileName>.c
  ./h

  ```
