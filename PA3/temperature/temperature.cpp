#include"temperature.h"
#include<cstdlib>
#include<cstdio>
#include<climits>
#include<cmath>

struct P{
    int x,y,t;
};

P points[200005];

struct Node{
    
    int minX,minY,maxX,maxY;//bounding box
    int min_t,max_t;//min/max temperature
    int x,y,t;//point data
    int l,r;//leftnode,rightnode
    
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
static int treeX[MAXN], treeY[MAXN], treeT[MAXN];//kdtree节点信息
static int minXArr[MAXN], maxXArr[MAXN];//bounding box
static int minYArr[MAXN], maxYArr[MAXN];//bounding box
static int minTArr[MAXN], maxTArr[MAXN];//bounding box temperature
static int ls[MAXN], rs[MAXN];//children


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
    treeX[me] = points[mid].x;
    treeY[me] = points[mid].y;
    treeT[me] = points[mid].t;
    minXArr[me] = maxXArr[me] = treeX[me];
    minYArr[me] = maxYArr[me] = treeY[me];
    minTArr[me] = maxTArr[me] = treeT[me];//tree[mid]存储points[mid]的信息，保证内存连续性
    ls[me] = buildKDTree(l, mid - 1, dim ^ 1);
    rs[me] = buildKDTree(mid + 1, r, dim ^ 1);
    //初始化bounding box和min/max temperature
    
    //更新bounding box
    if (ls[me] != -1) {
        int left = ls[me];
        if (minXArr[left] < minXArr[me]) minXArr[me] = minXArr[left];
        if (maxXArr[left] > maxXArr[me]) maxXArr[me] = maxXArr[left];
        if (minYArr[left] < minYArr[me]) minYArr[me] = minYArr[left];
        if (maxYArr[left] > maxYArr[me]) maxYArr[me] = maxYArr[left];
        if (minTArr[left] < minTArr[me]) minTArr[me] = minTArr[left];
        if (maxTArr[left] > maxTArr[me]) maxTArr[me] = maxTArr[left];
    }
    if (rs[me] != -1) {
        int right = rs[me];
        if (minXArr[right] < minXArr[me]) minXArr[me] = minXArr[right];
        if (maxXArr[right] > maxXArr[me]) maxXArr[me] = maxXArr[right];
        if (minYArr[right] < minYArr[me]) minYArr[me] = minYArr[right];
        if (maxYArr[right] > maxYArr[me]) maxYArr[me] = maxYArr[right];
        if (minTArr[right] < minTArr[me]) minTArr[me] = minTArr[right];
        if (maxTArr[right] > maxTArr[me]) maxTArr[me] = maxTArr[right];
    }
    return me;
}
static inline bool outside(int x1, int x2, int y1, int y2, int idx) {
    return (maxXArr[idx] < x1) || (minXArr[idx] > x2) || (maxYArr[idx] < y1) || (minYArr[idx] > y2);
}
static inline bool inside(int x1, int x2, int y1, int y2, int idx) {
    return (minXArr[idx] >= x1) && (maxXArr[idx] <= x2) && (minYArr[idx] >= y1) && (maxYArr[idx] <= y2);
}
static void queryRecurse(int idx, int x1, int x2, int y1, int y2, int& t1, int& t2, int& found){//递归查询
    //参数：idx子树根下标  x1x2y1y2查询矩形 t1t2结果温度范围 found是否找到过至少一个点
    if(idx==-1) return;
    if(outside(x1,x2,y1,y2,idx)) return;//完全不相交，剪枝
    if(inside(x1,x2,y1,y2,idx)){//包含
        if(!found){//define
            t1=minTArr[idx];
            t2=maxTArr[idx];
            found=1;
        }
        else{//update
            if(minTArr[idx]<t1) t1=minTArr[idx];
            if(maxTArr[idx]>t2) t2=maxTArr[idx];
        }
        return;
    }
    //检查当前节点
    if (treeX[idx] >= x1 && treeX[idx] <= x2 && treeY[idx] >= y1 && treeY[idx] <= y2) {
        if (!found) {
            t1 = t2 = treeT[idx];
            found = true;
        } else {
            if (treeT[idx] < t1) t1 = treeT[idx];
            if (treeT[idx] > t2) t2 = treeT[idx];
        }
    }
    //recurse
    if (ls[idx] != -1) queryRecurse(ls[idx], x1, x2, y1, y2, t1, t2, found);
    if (rs[idx] != -1) queryRecurse(rs[idx], x1, x2, y1, y2, t1, t2, found);
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

void query(int x1, int x2, int y1, int y2, int *tmin, int *tmax) {
    int t1=INT_MAX, t2=INT_MIN;
    int found=0;
    if(root!=-1)
        queryRecurse(root, x1, x2, y1, y2, t1, t2, found);
    if(found){
        *tmin=t1;
        *tmax=t2;
    }
    else{
        *tmin=-1;
        *tmax=-1;
    }
}