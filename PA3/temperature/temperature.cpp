#include"temperature.h"
#include<cstdlib>
#include<cstdio>
#include<climits>
#include<cmath>
#include<cassert>
#include<iostream>

struct P{
    int x,y,t;
};

P points[200005];

struct Node{
    
    int minX,minY,maxX,maxY;//bounding box
    int minT,maxT;//min/max temperature
    int x,y,t;//point data
    int ls,rs;//leftnode,rightnode
    
};
static inline void swapP(int a, int b) {//交换第a个和第b个点的值
    P tmp = points[a];
    points[a] = points[b];
    points[b] = tmp;
}

inline int min(int a, int b) {
    return (a < b) ? a : b;
}

inline int max(int a, int b) {
    return (a > b) ? a : b;
}

static inline bool cmpX(const P &a, const P &b) {
    return (a.x < b.x) || (a.x == b.x && a.y < b.y);
}


static inline bool cmpY(const P &a, const P &b) {
    return (a.y < b.y) || (a.y == b.y && a.x < b.x);
}

static const int MAXN=2e5+5;

Node t[MAXN];

int root=-1;
int N=0;//number of nodes

//quickselect
static int partition(int l, int r, int dim, const P &pivot) {//dim=0:x优先排序即vertical，dim=1:y优先排序即horizontal
    int i = l;
    for (int j = l; j < r; j++) {
        bool cond = (dim == 0 ? cmpX(points[j], pivot) : cmpY(points[j], pivot));
        if (cond) {
            swapP(i, j);//[l,i-1]均比pivot小
            i++;
        }
    }
    swapP(i, r);
    return i;//返回pivot应该在的位置
}


static void quickSelect(int l, int r, int k, int dim) {
    while (l < r) {
        int mid = (l + r) >> 1;
        // 使用中点作为 pivot
        P pivot = points[mid];
        swapP(mid, r);

        int pos = partition(l, r, dim, pivot);
        if (pos == k) return;
        else if (pos < k) l = pos + 1;//在右侧继续查找
        else r = pos - 1;//在左侧继续查找
    }
}

int buildKDTree(int l, int r, int dim) {//递归构建kdtree，dim偶0奇1
    if (l > r) return -1;
    int mid = (l + r) >> 1;
    quickSelect(l, r, mid, dim);
    int me=++N;
    t[me].x = points[mid].x;
    t[me].y = points[mid].y;
    t[me].t = points[mid].t;
    t[me].minX = t[me].maxX = t[me].x;
    t[me].minY = t[me].maxY = t[me].y;
    t[me].minT = t[me].maxT = t[me].t;//tree[mid]存储points[mid]的信息，保证内存连续性
    t[me].ls = buildKDTree(l, mid - 1, dim ^ 1);
    t[me].rs = buildKDTree(mid + 1, r, dim ^ 1);
    //初始化bounding box和min/max temperature
    
    //更新bounding box
    if (t[me].ls != -1) {
        int left = t[me].ls;
        if (t[left].minX < t[me].minX) t[me].minX = t[left].minX;
        if (t[left].maxX > t[me].maxX) t[me].maxX = t[left].maxX;
        if (t[left].minY < t[me].minY) t[me].minY = t[left].minY;
        if (t[left].maxY > t[me].maxY) t[me].maxY = t[left].maxY;
        if (t[left].minT < t[me].minT) t[me].minT = t[left].minT;
        if (t[left].maxT > t[me].maxT) t[me].maxT = t[left].maxT;
    }
    if (t[me].rs != -1) {
        int right = t[me].rs;
        if (t[right].minX < t[me].minX) t[me].minX = t[right].minX;
        if (t[right].maxX > t[me].maxX) t[me].maxX = t[right].maxX;
        if (t[right].minY < t[me].minY) t[me].minY = t[right].minY;
        if (t[right].maxY > t[me].maxY) t[me].maxY = t[right].maxY;
        if (t[right].minT < t[me].minT) t[me].minT = t[right].minT;
        if (t[right].maxT > t[me].maxT) t[me].maxT = t[right].maxT;
    }
    return me;
}
int lx, rx, ly, ry, t1, t2, found;
static inline bool outside(int lx, int rx, int ly, int ry, int idx) {
    return (t[idx].maxX < lx) || (t[idx].minX > rx) || (t[idx].maxY < ly) || (t[idx].minY > ry);
}
static inline bool inside(int lx, int rx, int ly, int ry, int idx) {
    return (t[idx].minX >= lx) && (t[idx].maxX <= rx) && (t[idx].minY >= ly) && (t[idx].maxY <= ry);
}
static void queryRecurse(int idx){//递归查询
    //参数：idx子树根下标  lxrxlyry查询矩形 t1t2结果温度范围 found是否找到过至少一个点
    if(idx==-1) return;
    if(outside(lx,rx,ly,ry,idx)) return;//完全不相交，剪枝
    if(found && t[idx].minT >= t1 && t[idx].maxT <= t2) return;//剪枝
    if(inside(lx,rx,ly,ry,idx)){//包含
        if(!found){//define
            t1=t[idx].minT;
            t2=t[idx].maxT;
            found=1;
        }
        else{//update
            if(t[idx].minT<t1) t1=t[idx].minT;
            if(t[idx].maxT>t2) t2=t[idx].maxT;
        }
        return;
    }
    //检查当前节点
    if (t[idx].x >= lx && t[idx].x <= rx && t[idx].y >= ly && t[idx].y <= ry) {
        if (!found) {
            t1 = t2 = t[idx].t;
            found = true;
        } else {
            if (t[idx].t < t1) t1 = t[idx].t;
            if (t[idx].t > t2) t2 = t[idx].t;
        }
    }
    //recurse
    if (t[idx].ls != -1) queryRecurse(t[idx].ls);
    if (t[idx].rs != -1) queryRecurse(t[idx].rs);
}


void init(int n, const int *x, const int *y, const int *t) {
    for (int i = 0; i < n; i++) {
        points[i].x = x[i];
        points[i].y = y[i];
        points[i].t = t[i];
    }
    if(n) root = buildKDTree(0, n - 1, 0);
    else root=-1;
}

void query(int lx, int rx, int ly, int ry, int *tmin, int *tmax) {
    t1=INT_MAX, t2=INT_MIN;
    found=0;
    if(root!=-1) {
        ::lx = lx;
        ::rx = rx;
        ::ly = ly;
        ::ry = ry;
        ::t1 = t1;
        queryRecurse(root);
    }
    if(found){
        *tmin=t1;
        *tmax=t2;
    }
    else{
        *tmin=-1;
        *tmax=-1;
    }
}