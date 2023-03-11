#ifndef DISKL_H_INCLUDED
#define DISKL_H_INCLUDED

#include <pthread.h>

pthread_mutex_t mut;
pthread_cond_t condVar;

#define lc ((n<<1)+1)
#define rc ((n<<1)+2)

#define N 256
int v[4*N], lazy[4*N];

void push(int n, int b, int e){
    // pthread_mutex_lock(&mut);
    if(lazy[n] == 0) return;
    v[n] += (e-b+1)*lazy[n];
    if(b!=e){
        lazy[lc] += lazy[n];
        lazy[rc] += lazy[n];
    }
    lazy[n] = 0;
    // pthread_mutex_unlock(&mut);
    return;
}

void build(int n, int b, int e){
    if(n == 0){
        pthread_mutex_init(&mut, NULL);
        pthread_cond_init(&condVar, NULL);
    }
    lazy[n] = 0;
    if(b == e){
        v[n] = 0;
        //a[b];
        return;
    }
    int mid = (b+e)>>1;
    build(lc, b, mid);
    build(rc, mid+1, e);
    v[n] = v[lc] + v[rc];
    return;
}

void update(int n, int b, int e, int i, int j, int val){
    // pthread_mutex_lock(&mut);
    push(n, b, e);
    if(b > j || e < i) return;
    if(i <= b && e <= j){
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

int allocate(int l, int r, int forceAllocate){
    pthread_mutex_lock(&mut);
    if(forceAllocate){
        while(query(0, 0, N-1, l, r) != 0){
            pthread_cond_wait(&condVar, &mut);
        }
        update(0, 0, N-1, l, r, 1);
        pthread_mutex_unlock(&mut);
        return 1;
    }
    if(query(0, 0, N-1, l, r) == 0){
        update(0, 0, N-1, l, r, 1);
        pthread_mutex_unlock(&mut);
        return 1;
    }
    else{
        pthread_mutex_unlock(&mut);
        return 0;
    }
}

void deallocate(int l, int r){
    pthread_mutex_lock(&mut);
    update(0, 0, N-1, l, r, -1);
    pthread_mutex_unlock(&mut);
    pthread_cond_signal(&condVar);
    return;
}

#endif
