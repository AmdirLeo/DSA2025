#include <cstdlib>
#include <cstdio>
using namespace std;

const int N=12000000;

unsigned int lc[N],rc[N];
unsigned int ltag[N];//懒标记
long long sum[N];//已经计算的和

long long n,m,cnt;

long long cal(unsigned int p, unsigned int l, unsigned int r){
    return sum[p]+(long long)ltag[p]*((long long)r-(long long)l+1);
}

void pushdown(unsigned int p, unsigned int l, unsigned int r){
    if(!ltag[p]) return;
    sum[p]=cal(p,l,r);
    if(l!=r){
        if(!lc[p]){
            lc[p]=++cnt;
        }
        if(!rc[p]){
            rc[p]=++cnt;
        }
        ltag[lc[p]]+=ltag[p];
        ltag[rc[p]]+=ltag[p];
    }  
    ltag[p]=0;
}

void update(unsigned int p, unsigned int l, unsigned int r, unsigned int ql, unsigned int qr){//l-r为节点总区间，ql-qr是要改的地方
    if(ql<=l && r<=qr){
        ltag[p]++;
        return;
    }
    if(r<ql || l>qr) return;
    pushdown(p,l,r);
    long long mid=(l+r)>>1;
    if(ql<=mid){
        if(!lc[p]){
            lc[p]=++cnt;
        }//创建左孩子，并修改
        update(lc[p],l,mid,ql,qr);
    }
    if(qr>mid){
        if(!rc[p]){
            rc[p]=++cnt;
        }//创建右孩子，并修改
        update(rc[p],mid+1,r,ql,qr);
    }
    sum[p]=(lc[p]?cal(lc[p],l,mid):0)+(rc[p]?cal(rc[p],mid+1,r):0);
}

long long query(unsigned int p, unsigned int l, unsigned int r, unsigned int ql, unsigned int qr){
    if(!p) return 0;
    if(ql<=l && r<=qr){
        return cal(p,l,r);
    }
    if(r<ql || l>qr) return 0;
    pushdown(p,l,r);
    long long mid=(l+r)>>1;
    long long result=0;
    if(ql<=mid){
        result+=query(lc[p],l,mid,ql,qr);
    }
    if(qr>mid){
        result+=query(rc[p],mid+1,r,ql,qr);
    }
    return result;
}
int main(){
    scanf("%lld%lld",&n,&m);
    cnt=1;//节点计数器
    for(int i=1;i<=m;i++){
        char op;
        scanf(" %c",&op);
        if(op=='H'){
            long long l,r;
            scanf("%lld%lld",&l,&r);
            update(1,1,n,l,r);
        }
        else{
            long long l,r;
            scanf("%lld%lld",&l,&r);
            printf("%lld\n",query(1,1,n,l,r));
        }
    }
    return 0;
}