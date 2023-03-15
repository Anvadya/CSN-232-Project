    #ifndef DISKL_H_INCLUDED
    #define DISKL_H_INCLUDED

    #include "semaphore.h"

    sem_t mut, condMut;

    //Defining the boundaries(left and right children) for the current node segment as macros.
    #define lc ((n<<1)+1)
    #define rc ((n<<1)+2)

    #define N 256
    //Defining the max-size of the segment tree.
    //In order to achieve a more efficient implementation, we use lazy-propagation.

    //When there are many updates to be performed on a range, we can postpone some updates 
    // and do those only when required.
    //If a segment tree's node's range lies within the update operation range, then all descendants
    //of the node must also be updated.

    //With lazy propagation, we update only the parent node, and postpone the updates to its children
    //by storing the update information in separate nodes called LAZY NODES.
    int v[4*N], lazy[4*N];

    //integers 'b' and 'e' represent the beginning and ending indices of the segment tree array under consideration.
    void push(int n, int b, int e){
        if(lazy[n] == 0) return;
        //If lazy flag is set for current node of segment tree, then there are some pending
        //updates. So, we need to make sure that the pending updates are done before processing 
        //the sub sum query.
        v[n] += (e-b+1)*lazy[n];
        if(b!=e){               //Checking for leaf node(b==e). If encountered => no more updates possible.
            lazy[lc] += lazy[n];
            lazy[rc] += lazy[n];
        }
        //Setting the lazy value of current node to 0, signifying that its update has been
        //completed.
        lazy[n] = 0;
        return;
    }


    //Function to recursively construct the segment tree representing disk space.
    //n is the index of current vertex, and 'b' and 'e' are the boundaries of current segment.
    void build(int n, int b, int e){
        if(n == 0){
            //Initialize the mutex and conditional variable for the root node.
            // pthread_mutex_init(&mut, NULL);
            // pthread_cond_init(&condVar, NULL);
            sem_init(&mut, 0, 1);
            sem_init(&condMut, 0, 0);
        }
        //Initialize the lazy flag to 0.
        lazy[n] = 0;
        if(b == e){
            v[n] = 0;
            //a[b];
            return;
        }
        //Recursively building the segment tree.
        int mid = (b+e)>>1;
        build(lc, b, mid);
        build(rc, mid+1, e);
        v[n] = v[lc] + v[rc];
        return;
    }



    //Below we define, the fundamental operations provided by a segment tree data structure - update() and query().
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

    int query(int n, int b, int e, int i, int j){
        // pthread_mutex_lock(&mut);
        push(n, b, e);
        if(i > e || j < b) return 0;
        if(i<=b && e<=j) return v[n];
        int mid = (b+e)>>1;
        // pthread_mutex_unlock(&mut);
        return (query(lc, b, mid, i, j) + query(rc, mid+1, e, i, j));
    }
    //forceAllocate added to ensure starvation-free implementation.
    // A request carrying forceAllocate will cause the calling thread to block on CondVar.
    //The caller thread will be granted the lock access on mut - Thus, will be granted space on the disk.
    int allocate(int l, int r, int forceAllocate){
        sem_wait(&mut);
        if(forceAllocate){
            while(query(0, 0, N-1, l, r) != 0){
                sem_wait(&condMut);
            }
            update(0, 0, N-1, l, r, 1);
            sem_post(&mut);
            return 1;
        }
        if(query(0, 0, N-1, l, r) == 0){
            update(0, 0, N-1, l, r, 1);
            sem_post(&mut);
            return 1;
        }
        else{
            sem_post(&mut);
            return 0;
        }
    }

    void deallocate(int l, int r){
        sem_wait(&mut);
        update(0, 0, N-1, l, r, -1);
        sem_post(&mut);
        sem_post(&condMut);
        return;
    }

    void destroy(){
        sem_destroy(&mut);
        sem_destroy(&condMut);
        return;
    }

    #endif
