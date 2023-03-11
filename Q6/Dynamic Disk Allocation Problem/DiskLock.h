#ifndef DISKL_H_INCLUDED
#define DISKL_H_INCLUDED


#define lc ((n<<1)+1)
#define rc ((n<<1)+2)

#define N 256
int v[4*N], lazy[4*N];

void push(int n, int b, int e){
    if(lazy[n] == 0) return;
    v[n] += (e-b+1)*lazy[n];
    if(b!=e){
        lazy[lc] += lazy[n];
        lazy[rc] += lazy[n];
    }
    lazy[n] = 0;
    return;
}

void build(int n, int b, int e){
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
    return;
}

int query(int n, int b, int e, int i, int j){
    push(n, b, e);
    if(i > e || j < b) return 0;
    if(i<=b && e<=j) return v[n];
    int mid = (b+e)>>1;
    return (query(lc, b, mid, i, j) + query(rc, mid+1, e, i, j));
}

#endif