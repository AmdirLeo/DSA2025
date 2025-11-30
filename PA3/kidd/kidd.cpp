#include <cstdio>
#include <cstdlib>


static const int N = 12000000;
static unsigned int lazytag[N];//懒标记，表示该节点区间需要增加的值
static long long sum[N];//该节点区间和
static unsigned int lc[N], rc[N];//左右孩子
long long n, m, flag=1;//flag为节点计数器
class LazySegTree {
private:
    

    inline long long compute(unsigned int p, unsigned int left, unsigned int right) {//计算节点p在区间[left,right]的真实值
        return sum[p] + (long long)lazytag[p] * ((long long)right - (long long)left + 1);
    }

    inline void push(unsigned int p, unsigned int left, unsigned int right) {//下传懒标记
        if (!lazytag[p]) return;

        sum[p] = compute(p, left, right);//将当前节点值更新
        if (left != right) {
            if (!lc[p]) lc[p] = ++flag;//不存在左右节点就创建
            if (!rc[p]) rc[p] = ++flag;
            lazytag[lc[p]] += lazytag[p];//下传懒标记
            lazytag[rc[p]] += lazytag[p];
        }
        lazytag[p] = 0;
    }

    void modify(unsigned int p, unsigned int left, unsigned int right, unsigned int qleft, unsigned int qright) {//区间修改，将区间[qleft,qright]加1
        if (qleft <= left && right <= qright) {//节点区间完全包含在修改区间中, 仅修改懒标记
            lazytag[p]++;
            return;
        }
        if (right < qleft || left > qright) return;//不在节点区间中

        push(p, left, right);//下传懒标记

        long long mid = (left + right) >> 1;
        if (qleft <= mid) {
            if (!lc[p]) lc[p] = ++flag;
            modify(lc[p], left, mid, qleft, qright);//修改左子树
        }
        if (qright > mid) {
            if (!rc[p]) rc[p] = ++flag;
            modify(rc[p], mid + 1, right, qleft, qright);//修改右子树
        }

        sum[p] = (lc[p] ? compute(lc[p], left, mid) : 0) + (rc[p] ? compute(rc[p], mid + 1, right) : 0);//更新当前节点值
    }

    long long query(unsigned int p, unsigned int left, unsigned int right, unsigned int qleft, unsigned int qright) {//查询基于节点p的区间[qleft,qright]的和

        if (!p) return 0;//节点不存在，返回0
        if (qleft <= left && right <= qright) return compute(p, left, right);//节点区间完全包含在查询区间中，返回节点值
        if (right < qleft || left > qright) return 0;//节点区间不在查询区间中，返回0

        push(p, left, right);//下传懒标记
        long long mid = (left + right) >> 1;
        long long result = 0;

        if (qleft <= mid) result += query(lc[p], left, mid, qleft, qright);//递归查询左子树
        if (qright > mid)  result += query(rc[p], mid + 1, right, qleft, qright);//递归查询右子树

        return result;
    }

public:
    LazySegTree() {}

    void read_input() {
        scanf("%lld%lld", &n, &m);
    }

    void run() {
        for (int i = 1; i <= m; ++i) {
            char op;
            scanf(" %c", &op);//注意前面的空格，跳过换行符
            long long left, right;
            scanf("%lld%lld", &left, &right);

            if (op == 'H') {
                modify(1, 1, n, left, right);
            } else {//op == 'Q'
                long long ans = query(1, 1, n, left, right);
                printf("%lld\n", ans);
            }
        }
    }
};


int main() {
    LazySegTree solver;
    solver.read_input();
    solver.run();
    return 0;
}
