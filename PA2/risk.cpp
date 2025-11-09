#include<cstdio>
#include<cstdlib>
#include<iostream>
using namespace std;

const int N = 1000005;
int n,T;
int x[N];
long long m[N];
long long k[N];
int dq[N];
int h = 0, t = 0;

int cmp(const void* a, const void* b) {
    long long x = *(long long*)a;
    long long y = *(long long*)b;
    if(x < y) return -1;
    else if(x > y) return 1;
    else return 0;
}

int binsearch(long long *arr, int left, int right, long long target) {
    int l = left, r = right + 1;
    while (l < r) {
        int mid = (l + r) >> 1;
        if (arr[mid] >= target) r = mid;
            else l = mid + 1;
    }
    return l; 
}

int main(){
    scanf("%d",&n);
    for(int i=1;i<=n;i++){
        scanf("%d",&x[i]);
    }
    for(int i=1;i<=n;i++){
        scanf("%lld",&m[i]);
    }
    scanf("%d",&T);
    for(int i=1;i<=n;i++){
        long long l = max((long long)1, (long long)i - m[i]);// left boundary
        while(h < t && dq[h] < l) h++;// pop front
        if(h < t) k[i]=x[dq[h]];
        else k[i]=0;// no valid max
        while(h < t && x[dq[t-1]] < x[i]) t--;// pop back
        dq[t++] = i;// push back
    }
    qsort(k+1,n,sizeof(long long),cmp);
    for(int t=0;t<T;t++){
        long long p,q;
        scanf("%lld%lld",&p,&q);

        int lpos=binsearch(k,1,n,p);
        int mpos=binsearch(k,1,n,q);
        printf("%d %d\n",lpos-1,mpos-lpos);
    }
    return 0;
}