# Sleeping Barbers Problem
## Implementation with Multiple Barbers, Multiple Customers and a Single Queue

The Sleeping Barbers Problem is a classical problem in computer science, which was introduced by Dijkstra in 1965. It describes a scenario where there are multiple barbers and multiple customers, but only one waiting room and one queue. The problem is to design a system that synchronizes the actions of the barbers and customers so that they don't interfere with each other and the system operates correctly without any deadlock or starvation.

### Problem Description

The problem can be described as follows:
* There are m barbers in a barber shop and n chairs in the waiting room.
* Customers enter the shop and check if there is an empty chair in the waiting room.
* If there is an empty chair, the customer sits down and waits for a barber to be available if all barbers is busy.
* If any barber is sleeping then the customer wakes him up to cut his hair.
* When a barber finishes cutting a customer's hair, the customer leaves and the barber goes to sleep until it is woken up again by a customer.
* If there are no customers waiting, the barber keeps on sleeping.

### Solution

There are several ways to solve the Sleeping Barbers Problem. One common approach is to use semaphores to coordinate the actions of the barbers and customers.
The solution involves using the following semaphores:
* `customers` - used to maintain the customers queue
* `barbers` - used to maintain the barbers action
* `mutex` - used to ensure mutual exclusion in the critical secction of code

The functions in the program can be summarised as:

#### `barberThread`

* Puts waiting barbers to sleep.<br />
* Selects the next customer for the haircut.<br />


#### `customerThread`

* Checks for availability of seats.<br />
* If a seat is available, it puts incoming customer on the seat.<br />
* Else the customer leaves the shop.<br />
  
## Conclusion

The Sleeping Barbers Problem is a classic problem in computer science that involves coordinating the actions of multiple threads to ensure that they don't interfere with each other. The solution involves using semaphores to ensure mutual exclusion and coordinate the actions of the barbers and customers.
