# DYNAMIC DISK ALLOCATION USING SEGMENT TREES

Segment trees can be used for efficient disk allocation by representing the disk space as 
a binary tree data structure. In this tree, each node represents a segment 
of the disk, and the children of a node represent the two halves of that segment.

* Each node in the segment tree can store information about the segment it represents,
such as its starting block and its size. 
* When a file needs to be allocated on the disk, we can traverse the segment tree to find a segment that is large enough to store
the file. This can be done by recursively traversing the tree and checking if the 
segment at each node is large enough to hold the file. 
* If a segment is found that is large enough, it can be marked as allocated by setting a flag in the node.
* When a file is deleted, its segment can be marked as free by clearing the allocation flag in the corresponding node. 
* To efficiently merge adjacent free segments, the segment tree can be augmented with additional information such as the number of free 
segments in each subtree, which can be updated during allocation and deallocation 
operations.
Segment trees can provide efficient disk allocation because they allow us to quickly 
find free segments that are large enough to store files, without having to search 
the entire disk space. They also allow us to efficiently merge adjacent free 
segments, reducing fragmentation and improving the overall utilization of the disk.

--- 

To realize a more efficient implementation of our solution, we will be using the concept 
of _LAZY PROPAGATION._

Lazy propagation is a technique that can be used to optimize operations on a segment tree, 
such as disk allocation. The basic idea behind lazy propagation is to delay updating the 
values of some nodes in the tree until they are actually needed. This can help reduce the 
number of operations required to update the tree, improving performance.

Thus we maintain yet another array - `lazy[]`.

```C
int v[4*N], lazy[4*N];
```
The idea is to initialize all elements of lazy[] as 0. 
> A value of 0 in lazy[i] indicates that there are no pending updates on node 'i' in the segment tree.
> A non-zero value means that this amount needs to be added to node i in segment tree before making an query to the node.

--- 

To use lazy propagation for disk allocation using segment trees, we can modify the allocation algorithm as follows:

1. When a file needs to be allocated on the disk, we start at the root node of the segment tree and recursively traverse the tree to find a segment that is large enough to store the file. We also keep track of a set of nodes that need to be updated to reflect the allocation of the file.

2. If we find a segment that is large enough to store the file, we mark it as allocated and add its corresponding node to the set of nodes to be updated.

3. When we reach a node that needs to be updated, we update its allocation status and set a flag indicating that its children also need to be updated. However, we do not immediately update the allocation status of the child nodes.

4. Instead, we propagate the update information down the tree, setting flags for nodes that need to be updated as we go.

5. When we need to perform an operation on a node that has update flags set, we update its child nodes first, propagating the update information down the tree until we reach nodes that have no update flags set.

6. Once we have updated all the child nodes, we can update the node itself and clear its update flags.

***

Hence we define the fundamental functions provided by Segment Trees - `update()` and `query()`

```C
void update(int n, int b, int e, int i, int j, int val){
    // pthread_mutex_lock(&mut);
    push(n, b, e);
    if(b > j || e < i) return;      //update range lies outside the range of current node.
    if(i <= b && e <= j){           //updating the lazy value to allow modification using the push() function.
        lazy[n] = val;
        push(n, b, e);
        return;
    }
    int mid = (b+e)>>1;   
    update(lc, b, mid, i, j, val);
    update(rc, mid+1, e, i, j, val);
    v[n] = v[lc] + v[rc];
    // pthread_mutex_unlock(&mut);
    return;
}

```
Fundamentally, `update()` operation allows us to update an element in the array and reflect
the corresponding change in segment tree. But, in reality, these updates can be done 
over a specified range for an array. 

Here, we recursively perform updates to ensure that the entire segment tree is affected by the query range
update, and the changes reflected.

Then, we have the `query()` function, which is used to determine whether a particular range of disk blocks is free or allocated.

```C
int query(int n, int b, int e, int i, int j){
    // pthread_mutex_lock(&mut);
    push(n, b, e);
    if(i > e || j < b) return 0;
    if(i<=b && e<=j) return v[n];
    int mid = (b+e)>>1;
    // pthread_mutex_unlock(&mut);
    return (query(lc, b, mid, i, j) + query(rc, mid+1, e, i, j));
}
```
> To perform a query in the segment tree for disk allocation, we start at the root of the tree and recursively traverse the tree
> to find the appropriate node that represents the range of disk blocks we are interested in. 
1. If the node represents a range of blocks that is entirely free, we can immediately return a positive result indicating that the entire range is available. 

2. If the node represents a range of blocks that is entirely allocated, we can immediately return a negative result indicating that the entire range is unavailable. 

3. If the node represents a range of blocks that is partially free and partially allocated, we continue recursively traversing the tree to check the children nodes until we have determined the availability of the entire range.

--- 

Consider the following function - 
The `push()` function does this - 
> For any queries, during the descent along the dree, we should always push information
> from the current vertex into both of its children.
It receives the current vertex, and it will push the information for its vertex to both
of its children - `lc` and `rc`.

```C
void push(int n, int b, int e){
    // pthread_mutex_lock(&mut);
    if(lazy[n] == 0) return;
    //If lazy flag is set for current node of segment tree, then there are some pending
    //updates. So, we need to make sure that the pending updates are done before processing 
    //the sub sum query.
    v[n] += (e-b+1)*lazy[n];
    if(b!=e){
        lazy[lc] += lazy[n];
        lazy[rc] += lazy[n];
    }
    lazy[n] = 0;
    // pthread_mutex_unlock(&mut);
    return;
}
```

If the lazy flag is set, we make the pending updates to this node.
This node indexed by n, i.e. `v[n]` represents the disk space bounded by `b` and `e`, 
and all these blocks must be increased by `lazy[n]`.

Then, we check if the caller node is not a leaf node (b==e), because if it is, then we have reached the end 
and cannot go any further as long as updation is concerned.

Hence come the statements 
```C
lazy[lc] += lazy[n];
lazy[rc] +=lazy[n];
```

Since we are not updating the children of Node `n` at this stage, we append the pending
updates to the corresponding `lazy[]` array entries.

Then finally, we set the lazy value for current node as 0, as its update us complete.




By using lazy propagation, we can avoid unnecessary updates to the segment tree and 
improve performance. When a file is deleted, we can still use the same deallocation 
algorithm as before, without the need for any modifications.
