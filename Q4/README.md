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
* `barbers` - used to maintain the barbers queue
* `mutex` - used to ensure mutual exclusion in the critical secction of code

The algorithm can be summarised as:

#### Barber

wait(`barbers`)
wait(`mutex`)
Select next customer and cut his hair
signal(`mutex`)
signal(`customers`)

#### Customer

wait(`mutex`)
if(free seat is available){
  Seat the customer
  signal(`mutex`)
  signal(`barbers`)
  wait(`customers`)
  //The barbers are woken up and the customers added to the customer queue.
}else{
  The customer leaves.
  signal(`mutex`)
}
  
## Conclusion

The Sleeping Barbers Problem is a classic problem in computer science that involves coordinating the actions of multiple threads to ensure that they don't interfere with each other. The solution involves using semaphores to ensure mutual exclusion and coordinate the actions of the barbers and customers. The solution is efficient, and the program runs without any issues.
