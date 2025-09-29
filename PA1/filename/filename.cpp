#include <iostream>
#include <string>
#include <cstdio>
using namespace std;

const int INF = 1e9;
string a, b;
int n, m, k;

int main() {

    cin >> n >> m >> k;
    
    cin >> a >> b;

    if (abs(n - m) > k) {
        cout << -1 << endl;
        return 0;
    }

    int prev[2 * k + 1], curr[2 * k + 1];//滚动数组
    for(int i = 0; i < 2 * k + 1; i++) {
        prev[i] = INF;
        curr[i] = INF;
    }

    // 初始化第 0 行
    for (int j = 0; j <= k && j <= m; j++) {
        prev[j + k] = j; 
        prev[-j + k] = j;
    }

    for (int i = 1; i <= n; i++) {
        int jmin = max(0, i - k);
        int jmax = min(m, i + k);

        for (int j = jmin; j <= jmax; j++) {
            int idx = j - i + k;
            curr[idx] = INF;

            if (j == 0) {
                curr[idx] = i; // 全删
            } else {
                // 删除
                if (j - i < k) {
                    curr[idx] = min(curr[idx], prev[idx + 1] + 1);
                }
                // 插入
                if (j - i > -k) {
                    curr[idx] = min(curr[idx], curr[idx - 1] + 1);
                }
                // 匹配
                if (a[i - 1] == b[j - 1]) {
                    curr[idx] = min(curr[idx], prev[idx]);                    
                }
            }
        }
        for(int j = 0; j < 2 * k + 1; j++) {
            prev[j] = curr[j];
        }

        bool flag = false;
        for (int j = jmin; j <= jmax; j++) {
            if (prev[j - i + k] <= k) {
                flag = true; 
                break;
            }
        }
        if (!flag) {
            cout << -1 << endl;
            return 0;
        }
    }

    if (prev[m - n + k] > k) cout << -1 << endl;
    else cout << prev[m - n + k] << endl;
    return 0;
}
