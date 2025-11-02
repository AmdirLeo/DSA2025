#include <cstdio>

int matrix[2001][2001];
double ssum[2001][2001];

int main() {
    int n, m, q;
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            scanf("%d", &matrix[i][j]);
            ssum[i][j] = ssum[i-1][j] + ssum[i][j-1] - ssum[i-1][j-1] + matrix[i][j];
        }
    }

    scanf("%d", &q);

    for (int i = 1; i <= q; ++i) {
        int x, y, a, b;
        scanf("%d %d %d %d", &x, &y, &a, &b);
        double sum = ssum[x+a-1][y+b-1] - ssum[x-1][y+b-1] - ssum[x+a-1][y-1] + ssum[x-1][y-1];
        printf("%.0f\n", sum);
    }
    return 0;
}