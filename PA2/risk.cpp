#include<cstdio>
#include<cstdlib>
using namespace std;

const int N = 1000005;
int n,T;
int x[N],m[N];
int k[N];
int dq[N];
int h = 0, t=0;

int cmp(const void* a, const void* b) {
    int x = *(const int*)a;
    int y = *(const int*)b;
    return (x > y) - (x < y);
}

int binsearch(int *arr, int left, int right, int target) {
    right++;
    while (left < right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] >= target) {
            right = mid;
        } else {
            left = mid + 1;
        }
    }
    return left;
}

int main(){
    scanf("%d",&n);
    for(int i=1;i<=n;i++){
        scanf("%d",&x[i]);
    }
    for(int i=1;i<=n;i++){
        scanf("%d",&m[i]);
    }
    scanf("%d",&T);
    for(int i=1;i<=n;i++){
        int l = i - m[i];// left boundary
        while(h < t && dq[h] < l) h++;// pop front
        while(h < t && x[dq[t-1]] <= x[i]) t--;// pop back
        dq[t++] = i;// push back
        k[i]=x[dq[h]];
    }
    qsort(k+1,n,sizeof(int),cmp);
    for(int t=0;t<T;t++){
        int p,q;
        scanf("%d%d",&p,&q);
        int lpos=binsearch(k,1,n,p);
        int mpos=binsearch(k,1,n,q);
        printf("%d %d\n",lpos-1,mpos-lpos);
    }
    return 0;
}