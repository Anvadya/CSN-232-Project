# Cigarette Smokers Problem

The cigarette smoker's problem is a well-known concurrency problem in Computer Science that was first described by Suhas Patil in 1971.The problem is to design a system that synchronizes the actions of the smokers and agent so that they don't interfere with each other and the system operates correctly without any deadlock or starvation.

### Problem Description

The problem involves:
* An agent (representing the Operating System)
* Three smokers (representing processes/threads)
* Each of whom has an infinite supply of one of the three ingredients required to make and smoke a cigarette - tobacco, paper, and matches
The problem can be describes as:
* The agent randomly selects two of the ingredients to place on the table/
* The smoker with the third ingredient makes a cigarette with their supply and the two on the table, then smokes it.
* This process repeats indefinitely, with the agent placing two new items on the table each time.

### Solution
The code utilizes three semaphores to represent the three smokers `sem_for_smokers[3]`. The agent also uses another semaphore `sem_for_agents` to increase the corresponding semaphore, indicating that an item has been placed on the table. Each smoker has their own associated semaphore `smokerReady`, which they use to signal to the agent when they have finished smoking. The agent, in turn, has a semaphore `agent_is_ready` that waits on each smoker's semaphore to indicate when it can place new items on the table. This process continues indefinitely, although for testing purposes, each smoker's turns are limited to in the `const int cigarattes_to_be_smoked_initially`, resulting in `3*cigarattes_to_be_smoked_initially` item placements by the agent
## Conclusion
The Cigarette Smokers  Problem is a classic problem in computer science that involves coordinating the actions of multiple threads(agents) to ensure that they don't interfere with each other. The solution involves using semaphores to ensure mutual exclusion and coordinate the actions of the smokers and agent.
