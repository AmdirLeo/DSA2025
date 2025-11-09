#include<iostream>
#include<cstdio>

using namespace std;


int n,m;
int root;

struct Node {
    int parent,nxt,prev,firc,lstc,sz,cd,dp=0;//父亲节点、下一个兄弟、前一个兄弟、第一个儿子、最后一个儿子、子树大小、子节点数量、子树高度
    int sufmax;//后缀最大值
} a[1000005];

int initsz(int x){
    if(a[x].cd==0) return 1;//
    for(int i=a[x].firc;i!=-1;i=a[i].nxt){
        a[x].sz+=initsz(i);
    }
    return a[x].sz;
}

int initdp(int x){
    if(a[x].cd==0) return 0;//
    for(int i=a[x].firc;i!=-1;i=a[i].nxt){
        a[x].dp=max(a[x].dp, initdp(i)+1);
    }
    return a[x].dp;
}
void initsufmax(int x){
    if(a[x].cd==0) return;
    int tmp=-1;
    for(int i=a[x].lstc;i!=-1;i=a[i].prev){
        a[i].sufmax=max(tmp,a[i].dp);
        tmp=a[i].sufmax;
        initsufmax(i);
    }
    return;
}

int gotoIdx(){
    int x;
    scanf("%d",&x);
    int curr=root;
    int flag=0;
    for(int i=0;i<x;i++){
        int r;
        scanf("%d",&r);
        if(flag) continue;
        if(a[curr].cd<r+1){
            flag=1;
            continue;
        }
        curr=a[curr].firc;
        while(r--){
            curr=a[curr].nxt;
        }
    }
    return curr;
}

int getdp(int x){
    if(!a[x].cd) return 0;
    else return a[a[x].firc].sufmax+1;
}

void flash(int x){
    int curr=x;
    while(curr){
        while(1){
            a[curr].sufmax=max(getdp(curr),a[curr].nxt!=-1?a[a[curr].nxt].sufmax:0);
            if(a[curr].prev==-1){
                break;
            }
            curr=a[curr].prev;
        }
        curr=a[curr].parent;
    }
}//刷新后缀最大高度

void secede(int x){
    if(x==root) return;
    int parent=a[x].parent;
    for(int i=parent;i!=0;i=a[i].parent){
        a[i].sz-=a[x].sz;
    }
    if(a[parent].cd==1){
        a[parent].firc=a[parent].lstc=-1;
        a[parent].cd=0;
        flash(parent);
    }
    else if(a[parent].firc==x){
        a[parent].cd--;
        a[a[x].nxt].prev=-1;
        a[parent].firc=a[x].nxt;
        flash(parent);
    }
    else if(a[parent].lstc==x){
        a[parent].cd--;
        a[a[x].prev].nxt=-1;
        a[parent].lstc=a[x].prev;
        flash(a[x].prev);
    }
    else{
        a[parent].cd--;
        a[a[x].prev].nxt=a[x].nxt;
        a[a[x].nxt].prev=a[x].prev;
        flash(a[x].prev);
    }
}

void attach(int s,int d,int k){
    a[s].parent=d;
    for(int i=d;i!=0;i=a[i].parent){
        a[i].sz+=a[s].sz;
    }
    if(a[d].cd==0){
        a[d].cd++;
        a[d].firc=a[d].lstc=s;
        a[s].prev=a[s].nxt=-1;
        flash(s);
    }
    else if(k==0){
        a[d].cd++;
        a[a[d].firc].prev=s;
        a[s].nxt=a[d].firc;
        a[d].firc=s;
        a[s].prev=-1;
        flash(s);
    }
    else if(k==a[d].cd){
        a[d].cd++;
        a[a[d].lstc].nxt=s;
        a[s].prev=a[d].lstc;
        a[s].nxt=-1;
        a[d].lstc=s;
        
        flash(s);
    }
    else{
        int curr=a[d].firc;
        while(k--){
            curr=a[curr].nxt;
        }
        a[d].cd++;
        a[a[curr].prev].nxt=s;
        a[s].prev=a[curr].prev;
        a[s].nxt=curr;
        a[curr].prev=s;
        flash(s);

    }
}

int main(){
    scanf("%d%d",&n,&m);
    for(int i = 1; i <= n; i++){
        a[i].sz = 1;
        int count;
        scanf("%d",&count);
        a[i].cd=count;
        if(count == 0){
            a[i].firc = -1;
            a[i].lstc = -1;
            continue;
        }
        if(count == 1){
            int x;
            scanf("%d",&x);
            a[x].parent = i;
            a[x].prev = a[x].nxt = -1;
            a[i].firc=a[i].lstc=x;
            continue;
        }
        int x;
        int flag;
        scanf("%d",&x);
        a[x].parent=i;
        a[x].prev=-1;
        a[i].firc=x;
        flag=x;
        for(int j=2;j<count;j++){
            scanf("%d",&x);
            a[x].parent=i;
            a[x].prev=flag;
            a[flag].nxt=x;
            flag=x;
        }
        scanf("%d",&x);
        a[x].parent=i;
        a[x].prev=flag;
        a[flag].nxt=x;
        a[x].nxt=-1;
        a[i].lstc=x;
    }

    for(int i=1;i<=n;i++){
        if(a[i].parent==0){
            root=i;
            break;
        }
    }
    a[root].prev=a[root].nxt=-1;
    
    initsz(root);
    initdp(root);
    a[root].sufmax=a[root].dp;
    initsufmax(root);
    for(int i=1;i<=m;i++){
        int t;
        scanf("%d",&t);
        if(t==0){
            int s=gotoIdx();
            secede(s);
            int d=gotoIdx();
            int k;
            scanf("%d",&k);
            attach(s,d,k);
        }
        if(t==1){
            int x=gotoIdx();
            printf("%d\n", getdp(x));
        }
        if(t==2){
            int x=gotoIdx();
            printf("%d\n",a[x].sz);
        }
    }
    return 0;
}