
#include <cstdlib>
#include <cstring>
#include"feature.h"

struct data{
    int w;
    int x;
    int idx;
}; 
struct Node{
    int w;//weight
    int x;
    int idx;//order
    long long sum;//子树和
    int sz;//subtreesize
    Node* p;
    Node* lc;
    Node* rc;
    Node(const int &p) :w(p),x(0),sum(0),sz(0),p(nullptr),lc(nullptr),rc(nullptr){}
};


int N;
int X[500005];
int W[500005];
data dset[500005];

Node* A; //normal
Node* B; //absolute

int cmp(const void*a,const void*b){
    const data* A = static_cast<const data*>(a);
    const data* B = static_cast<const data*>(b);
    return A->w - B->w;
}

int cmpabs(const void*a,const void*b){
    const data* A = static_cast<const data*>(a);
    const data* B = static_cast<const data*>(b);
    return abs(A->w) - abs(B->w);
}

Node* initTree(int l,int r,Node*p){
    if(l>r) return nullptr;
    int mid=(l+r)>>1;
    Node* node=new Node(dset[mid].w);
    node->idx=dset[mid].idx;
    node->x=dset[mid].x;
    node->sz=r-l+1;
    node->p=p;
    node->lc=initTree(l,mid-1,node);
    node->rc=initTree(mid+1,r,node);
    node->sum=((long long)node->w)*((long long)node->x);
    if(node->lc) node->sum+=node->lc->sum;
    if(node->rc) node->sum+=node->rc->sum;
    return node;
}


void init(int n, const int *w, const int *x){
    N=n;
    for(int i=1;i<=n;i++){
        X[i]= *(x+i);
        W[i]= *(w+i);
        dset[i].x= *(x+i);
        dset[i].w= *(w+i);
        dset[i].idx=i;
    }
    qsort(dset + 1, n, sizeof(data), cmp);
    A = initTree(1, n, nullptr);
    qsort(dset + 1, n, sizeof(data), cmpabs);
    B = initTree(1, n, nullptr);
}

void zig(Node* node){//左孩子提升为新的根
    if(node->rc) node->rc->p=node->p;
    node->p->lc=node->rc;
    node->rc=node->p;
    node->p=node->rc->p;
    node->rc->p=node;
    if(node->p){
        if(node->p->lc==node->rc){
            node->p->lc=node;
        }
        else node->p->rc=node;
    }
    node->sum=node->rc->sum;
    node->sz=node->rc->sz;
    node->rc->sum = node->sum - (long long)node->w * (long long) node->x;
    node->rc->sz=node->sz-1;
    if (node->lc) {
        node->rc->sum-= node->lc->sum;
        node->rc->sz -= node->lc->sz;
    }

}

void zag(Node* node){//相反
    if(node->lc) node->lc->p=node->p;
    node->p->rc=node->lc;
    node->lc=node->p;
    node->p=node->lc->p;
    node->lc->p=node;
    if(node->p){
        if(node->p->rc==node->lc){
            node->p->rc=node;
        }
        else node->p->lc=node;
    }
    node->sum=node->lc->sum;
    node->sz=node->lc->sz;
    node->lc->sum = node->sum - (long long)node->w * (long long) node->x;
    node->lc->sz=node->sz-1;
    if (node->rc) {
        node->lc->sum -= node->rc->sum;
        node->lc->sz -= node->rc->sz;
    }
}

void emerge(Node* node){
    while(node->p){
        if(node->p->p){
            if(node->p->p->lc==node->p){
                if(node->p->lc==node){
                    zig(node->p);
                    zig(node);
                }
                else{
                    zag(node);
                    zig(node);
                }
            }else{
                if(node->p->lc==node){
                    zig(node);
                    zag(node);
                }
                else{
                    zag(node->p);
                    zag(node);
                }
            }
            continue;
        }
        if(node->p){
            if(node->p->lc==node) zig(node);
            else zag(node);
            continue;
        }
    }
    return;
}

Node* search(Node* root, int w){
    if(!root) return nullptr;
    if(root->w==w) return root;
    if(root->w>w){
        if(root->lc) return search(root->lc,w);
        else return root;
    }
    else{
        if(root->rc) return search(root->rc,w);
        return root;
    }
}

Node* searchabs(Node* root, int w){
    if(!root) return nullptr;
    if(abs(root->w)==abs(w)) return root;
    if(abs(root->w)>abs(w)){
        if(root->lc) return searchabs(root->lc,w);
        else return root;
    }
    else{
        if(root->rc) return searchabs(root->rc,w);
        return root;
    }
}

Node* searchidx(Node* root, int k) {//第k个
    if (!root) return nullptr;
    if (!root->lc) {
        if (k == 1) return root;
        else return searchidx(root->rc, k - 1);
    } else {
        if (k <= root->lc->sz)
            return searchidx(root->lc, k);
        else if (k == root->lc->sz + 1)
            return root;
        else return searchidx(root->rc, k - root->lc->sz - 1);
    }
}

Node* searchmin(Node* root) {//最小值
    if (!root) return nullptr;
    if (!root->lc) return root;
    else return searchmin(root->lc);
}



void modify_weight(int i, int w){
    int t=*(W+i);
    *(W+i)=w;
    Node* node=search(A,t);
    emerge(node);
    A=node;
    node->p=nullptr;
    Node* T1=node->lc;
    Node* T2=node->rc;
    if(T1) T1->p=nullptr;
    if(T2) T2->p=nullptr;
    if(!T2){
        A=T1;
        T1->p=nullptr;
    }
    else{
        Node* tmp=searchmin(T2);
        emerge(tmp);
        A=tmp;
        tmp->lc=T1;
        if(T1){
            T1->p=tmp;
            tmp->sz+=T1->sz;
            tmp->sum+=T1->sum;
        }
    }
    long long delta;
    Node* start;

    //初始化新节点
    Node* nodein = new Node(w);
    //设置需要更新的总和值
    delta = (long long) w * (long long)X[i];

    nodein->x = X[i];
    nodein->idx = i;

    Node* position = search(A, w);
    if (position->w > w)
        position->lc = nodein;
    else
        position->rc = nodein;
    nodein->p = position;
    start = nodein;

    do {
        start->sz = start->sz + 1;
        start->sum = start->sum + delta;
        start = start->p;

    } while (start != nullptr);

    //将新节点升到最高位置
    emerge(nodein);
    A = nodein;
    //对B同理
    node=searchabs(B,t);
    emerge(node);
    B=node;
    node->p=nullptr;
    T1=node->lc;
    T2=node->rc;
    if(T1) T1->p=nullptr;
    if(T2) T2->p=nullptr;
    if(!T2){
        B=T1;
        T1->p=nullptr;
    }
    else{
        Node* tmp=searchmin(T2);
        emerge(tmp);
        B=tmp;
        tmp->lc=T1;
        if(T1){
            T1->p=tmp;
            tmp->sz+=T1->sz;
            tmp->sum+=T1->sum;
        }
    }
    Node* nodeinabs = new Node(w);

    nodeinabs->x = X[i];
    nodeinabs->idx = i;

    position = searchabs(B, w);
    if (abs(position->w) > abs(w))
        position->lc = nodeinabs;
    else
        position->rc = nodeinabs;
    nodeinabs->p = position;
    start = nodeinabs;

    do {
        start->sz = start->sz + 1;
        start->sum = start->sum + delta;
        start = start->p;
    } while (start != nullptr);
    emerge(nodeinabs);
    B = nodeinabs;
}

void modify_value(int k, int x){
    long long tmp;
    long long delta;
    Node* node = searchidx(A, N - k + 1);

    long long w = node->w;
    tmp = (long long)node->x;
    delta = (long long)(x - tmp) * (long long)node->w;

    //更新数据
    node->x = x;
    X[node->idx] = x;
    do {
        node->sum = node->sum + delta;
        node = node->p;
    } while (node != nullptr);
    //B
    node = searchabs(B, w);
    node->x = x;
    do {
        node->sum = node->sum + delta;
        node = node->p;
    } while (node != nullptr);
    return;
}

long long calculate(int k){
    long long res;
    Node* node = searchidx(B, N - k + 1);
    emerge(node);
    B = node;

    res = (long long)B->w * (long long)B->x;

    if (k > 1)
        res+= B->rc->sum;
    return res;
}
