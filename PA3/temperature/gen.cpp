#include <bits/stdc++.h>
using namespace std;
mt19937 rng;
int main() {
    int n = 200000, m = 160000;
    printf("%d\n", n);
    for (int i = 0; i < n; i++) {
        printf("%d %d %d\n", rng() % 100000000, rng() % 100000000, rng() % 100);
    }
    for (int i = 0; i < m; i++) {
        int x1 = rng() % 100000000, x2 = rng() % 100000000,
            y1 = rng() % 100000000, y2 = rng() % 100000000;
        if (x1 > x2) swap(x1, x2);
        if (y1 > y2) swap(y1, y2);
        printf("%d %d %d %d\n", x1, x2, y1, y2);
    }
}