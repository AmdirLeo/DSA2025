#include <cstdlib>
#include <cstring>
#include"feature.h"
#include<cstdio>
struct data{
    int w;
    int x;
    int idx;
}; 

int N;//number of elements
int X[500005];//x
int W[500005];//w
data dset[500005];//use for sort
inline int max(int a,int b){
    return a>b?a:b;
}
class Node{
public:
    int w;//weight
    int x;
    int idx;//order
    int h;//height
    long long sum;//子树和
    int sz;//subtreesize
    Node* p;
    Node* lc;
    Node* rc;
    Node(const int &p) :w(p),x(0),h(1),sum(0),sz(1),p(nullptr),lc(nullptr),rc(nullptr){}
    static Node* initTree(int l,int r,Node*p){
        if(l>r) return nullptr;
        int mid=(l+r)>>1;
        Node* node=new Node(dset[mid].w);//root
        node->idx=dset[mid].idx;
        node->x=dset[mid].x;
        node->sz=r-l+1;
        node->p=p;
        node->lc=initTree(l,mid-1,node);//recurse
        node->rc=initTree(mid+1,r,node);
        node->sum=((long long)node->w)*((long long)node->x);
        if(node->lc) node->sum+=node->lc->sum;
        if(node->rc) node->sum+=node->rc->sum;
        node->h=1+max(node->lc?node->lc->h:0,node->rc?node->rc->h:0);
        return node;
    }
    int height (Node* node) {
        return node ? node->h : 0;
    }
    int balanceFactor(Node* node) {
        return node ? height(node->lc) - height(node->rc) : 0;
    }

    void update(Node* node) {
        if (node) {
            node->h = 1 + max(height(node->lc), height(node->rc));
            node->sz = 1 + (node->lc?node->lc->sz:0) + (node->rc?node->rc->sz:0);
            node->sum = ((long long)node->w) * ((long long)node->x);
            if (node->lc) node->sum += node->lc->sum;
            if (node->rc) node->sum += node->rc->sum;
        }
    }
    Node* rotateRight(Node* y) {//rotate
        Node* x = y->lc;
        Node* T2 = x->rc;

        x->rc = y;
        y->lc = T2;
        x->p=y->p;
        y->p=x;
        if(T2) T2->p=y;
        if(x->p){
            if(x->p->lc==y) x->p->lc=x;
            else x->p->rc=x;
        }
        update(y);
        update(x);

        return x;//new root
    }
    Node* rotateLeft(Node* x) {
        Node* y = x->rc;
        Node* T2 = y->lc;

        y->lc = x;
        x->rc = T2;
        y->p=x->p;
        x->p=y;
        if(T2) T2->p=x;
        if(y->p){
            if(y->p->lc==x) y->p->lc=y;
            else y->p->rc=y;
        }

        update(x);
        update(y);

        return y;
    }
    Node* balance(Node* node) {
        update(node);
        int balance = balanceFactor(node);
        if (balance > 1) {//left heavy
            if (balanceFactor(node->lc) < 0) {
                node->lc = rotateLeft(node->lc);
            }
            return rotateRight(node);
        }
        if (balance < -1) {//right heavy
            if (balanceFactor(node->rc) > 0) {
                node->rc = rotateRight(node->rc);
            }
            return rotateLeft(node);
        }
        return node;
    }
    Node* insertA(Node* node, int w, int x, int i) {
        if (!node){//没有就创建新的
            Node* n=new Node(w);
            n->x=x;
            n->idx=i;
            n->sum=(long long)w*(long long)x;
            return n;
        }
        if (w < node->w) {//recurse
            node->lc = insertA(node->lc, w, x, i);
            if(node->lc) node->lc->p=node;
        } else if (w > node->w) {//recurse
            node->rc = insertA(node->rc, w, x, i);
            if(node->rc) node->rc->p=node;
        } else {//no equal
            return node;
        }
        return balance(node);//balance
    }
    Node* insertB(Node* node, int w, int x, int i) {//abs版本
        if (!node){
            Node* n=new Node(w);
            n->x=x;
            n->idx=i;
            n->sum=(long long)w*(long long)x;
            return n;
        }
        if (abs(w) < abs(node->w)) {//recurse
            node->lc = insertB(node->lc, w, x, i);
            if(node->lc) node->lc->p=node;
        } else if (abs(w) > abs(node->w)) {//recurse
            node->rc = insertB(node->rc, w, x, i);
            if(node->rc) node->rc->p=node;
        } else {//no equal
            return node;
        }
        return balance(node);//balance
    }
    Node* searchA(Node* root, int w){//search for w
        if(!root) return nullptr;
        if(root->w==w) return root;
        if(root->w>w){
            return searchA(root->lc,w);
        }
        else{
            return searchA(root->rc,w);
        }
    }
    Node* searchB(Node* root, int absw){//search for |w|
        if(!root) return nullptr;
        if(abs(root->w)==absw) return root;
        if(abs(root->w)>absw){
            return searchB(root->lc,absw);
        }
        else{
            return searchB(root->rc,absw);
        }
    }
    Node* searchk(Node* root, int k){//search for kth biggest
        if(!root) return nullptr;
        int rsz=root->rc?root->rc->sz:0;
        if(k==rsz+1) return root;//hit
        else if(k<=rsz) return searchk(root->rc,k);//bigger,go right
        else return searchk(root->lc,k-rsz-1);//smaller,go left
    }
    inline Node* minValueNode(Node* node) {//子树最小值
        Node* current = node;
        while (current->lc) {
            current = current->lc;
        }
        return current;
    }

    Node* deleteA(Node* root, int w) {
        if (!root) return root;//empty

        if (w < root->w) {//recurse
            root->lc = deleteA(root->lc, w);
            if(root->lc) root->lc->p=root;
        } else if (w > root->w) {//recurse
            root->rc = deleteA(root->rc, w);
            if(root->rc) root->rc->p=root;
        } else {//found
            if (!root->lc || !root->rc) {//单分支
                Node* child = root->lc ? root->lc : root->rc;//get child
                if (!child) {//no child?
                    delete root;
                    return nullptr;
                } else {//copy child
                    child->p = root->p;
                    delete root;
                    return child;
                }
            } else {//two children
                Node* temp = minValueNode(root->rc);//后继
                root->w = temp->w;//copy value
                root->x = temp->x;
                root->idx = temp->idx;
                root->rc = deleteA(root->rc, temp->w);//downstream delete
                if(root->rc) root->rc->p=root;
            }
        }
        if (!root) return root;

        return balance(root);
    }
    Node* deleteB(Node* root, int w) {
        if (!root) return root;//empty

        if (abs(w) < abs(root->w)) {//recurse
            root->lc = deleteB(root->lc, w);
            if(root->lc) root->lc->p=root;
        } else if (abs(w) > abs(root->w)) {//recurse
            root->rc = deleteB(root->rc, w);
            if(root->rc) root->rc->p=root;
        } else {//found
            if (!root->lc || !root->rc) {//单分支
                Node* child = root->lc ? root->lc : root->rc;//get child
                if (!child) {//no child?
                    delete root;
                    return nullptr;
                } else {//copy child
                    child->p = root->p;
                    delete root;
                    return child;
                }
            } else {//two children
                Node* temp = minValueNode(root->rc);//后继
                root->w = temp->w;//copy value
                root->x = temp->x;
                root->idx = temp->idx;
                root->rc = deleteB(root->rc, temp->w);//downstream delete
                if(root->rc) root->rc->p=root;
            }
        }
        if (!root) return root;

        return balance(root);
    }

};



int cmp(const void*a,const void*b){//compare functions for qsort
    const data* A = static_cast<const data*>(a);
    const data* B = static_cast<const data*>(b);
    return A->w - B->w;
}

int cmpabs(const void*a,const void*b){
    const data* A = static_cast<const data*>(a);
    const data* B = static_cast<const data*>(b);
    return abs(A->w) - abs(B->w);
}



Node* A; //normal
Node* B; //absolute

void init(int n, const int *w, const int *x){
    N=n;
    for(int i=1;i<=n;i++){//read input
        X[i]= *(x+i);
        W[i]= *(w+i);
        dset[i].x= *(x+i);
        dset[i].w= *(w+i);
        dset[i].idx=i;
    }
    qsort(dset + 1, n, sizeof(data), cmp);
    A=Node::initTree(1, n, nullptr);
    qsort(dset + 1, n, sizeof(data), cmpabs);
    B=Node::initTree(1, n, nullptr);
}
void print_inorder(Node* root) {//test
    if(!root) return;
    print_inorder(root->lc);
    printf("idx=%d w=%d x=%d |w|=%d sum=%lld sz=%d\n",
           root->idx, root->w, root->x, abs(root->w),
           (long long)root->sum, root->sz);
    print_inorder(root->rc);
}


//修改权重，对A和B更新，先删掉旧的节点，再插入新的节点
void modify_weight(int i, int wnew){
    int wold=W[i];
    W[i]=wnew;
    A=A->deleteA(A,wold);
    A=A->insertA(A,wnew,X[i],i);
    B=B->deleteB(B,wold);
    B=B->insertB(B,wnew, X[i],i);
}

void modify_value(int k, int xnew){
    Node* node=A->searchk(A,k);//第k大
    int w=node->w;
    
    long long d=(long long)(xnew-(node->x))*(long long)w;//计算要改的差值
    X[node->idx]=xnew;
    node->x=xnew;
    while(node){
        
        node->sum+=d;
        node=node->p;
    }
    node=B->searchB(B,abs(w));//B同理
    node->x=xnew;
    while(node){
        node->sum+=d;
        node=node->p;
    }
    return;
}

long long sumTopK(Node* root, int k) {//计算树前k大的sum
    if (!root || k <= 0) return 0;
    int rsz = root->rc ? root->rc->sz : 0;
    long long rightSum = root->rc ? root->rc->sum : 0;
    long long self = (long long)root->w * (long long)root->x;
    if (k <= rsz) {
        return sumTopK(root->rc, k);
    } else if (k == rsz + 1) {
        return rightSum + self;
    } else { // k > rsz + 1
        return rightSum + self + sumTopK(root->lc, k - rsz - 1);
    }
}

long long calculate(int k){//计算前k大权重的和
    return sumTopK(B, k);    
}
