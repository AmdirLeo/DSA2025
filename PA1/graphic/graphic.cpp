#include <cstdio>
#include <cstdlib>

using namespace std;

long long A[200001];
long long B[200001];
long long tmp[200001];

inline bool check(int i, long long x, long long y){
    __int128 left = (__int128)A[i] * (__int128)B[i];
    __int128 right = (__int128)B[i] * (__int128)x + (__int128)A[i] * (__int128)y;
    return left <= right;
}

void merge_sort(long long A[], int l, int r){
    if(l >= r) return;
    int mid = (l + r) >> 1;
    merge_sort(A, l, mid);
    merge_sort(A, mid + 1, r);
    int i = l, j = mid + 1, k = 0;
    
    while(i <= mid && j <= r){
        if(A[i] <= A[j]) tmp[k++] = A[i++];
        else tmp[k++] = A[j++]; 
    }
    while(i<=mid) tmp[k++] = A[i++];
    while(j<=r) tmp[k++] = A[j++];
    for(int t=l; t<=r; ++t) A[t] = tmp[t-l];

}

int main() {
    int n; 
    scanf("%d", &n);
    for(int i = 0; i < n; i++) scanf("%lld", &A[i]);
    for(int i = 0; i < n; i++) scanf("%lld", &B[i]);
    merge_sort(A, 0, n - 1);
    merge_sort(B, 0, n - 1);
    int m;
    scanf("%d", &m);
    for(int i = 0; i < m; i++){
        long long x, y;
        scanf("%lld %lld", &x, &y);
        int l = 0, r = n - 1, ans = -1;
        while(l <= r){
            int mid = (l + r) >> 1;
            if(check(mid, x, y)){
                ans = mid;
                l = mid + 1;
            }
            else r = mid - 1;
        }
        printf("%d\n", ans + 1);
    }
    return 0;
}