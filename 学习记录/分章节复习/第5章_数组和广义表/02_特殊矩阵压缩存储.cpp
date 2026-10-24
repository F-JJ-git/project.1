// 第5章：对称、下三角、三对角矩阵的压缩存储。
// 课件：幻灯片22～31；特别注意23、26、29页的数组下标和公共常量位置。
// 矩阵行列号从1开始，压缩数组从0开始。下三角/三对角的a[0]存公共常量c。
// 输入：先显示固定示例；随后输入类型(1/2/3)和行列号查询，输入0或EOF结束。
// 单次设置/取值O(1)；对称/下三角空间O(n^2)，三对角空间O(n)。
#include <iostream>
#include <vector>
#include <stdexcept>
#include <utility>
#include <cstdlib>
#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif
using namespace std;

int ValidOrder(int n)
{
    if (n < 1 || n > 1000) throw invalid_argument("示例阶数须在1至1000之间");
    return n;
}

void CheckPosition(int n, int i, int j)
{
    if (i < 1 || i > n || j < 1 || j > n) throw out_of_range("下标越界");
}

class SymmetricMatrix {
    int n;
    vector<int> a;
    int Index(int i, int j) const {
        if (i < j) swap(i, j); // a(i,j)=a(j,i)，统一映射到下三角。
        // 前i-1行存了1+...+(i-1)项，本行j前面还有j-1项。
        return i * (i - 1) / 2 + j - 1;
    }
public:
    explicit SymmetricMatrix(int order)
        : n(ValidOrder(order)), a(n * (n + 1) / 2, 0) {}
    void Set(int i, int j, int value) {
        CheckPosition(n, i, j); a[Index(i, j)] = value;
    }
    int Get(int i, int j) const {
        CheckPosition(n, i, j); return a[Index(i, j)];
    }
    void ShowStorage() const { for (int x : a) cout << x << ' '; cout << '\n'; }
};

class LowerTriangularMatrix {
    int n;
    vector<int> a;
public:
    explicit LowerTriangularMatrix(int order, int common = 0)
        : n(ValidOrder(order)), a(n * (n + 1) / 2 + 1, 0) { a[0] = common; }
    void Set(int i, int j, int value) {
        CheckPosition(n, i, j);
        if (i < j) {
            if (value != a[0]) throw invalid_argument("上三角只能取公共常量c");
            return;
        }
        a[i * (i - 1) / 2 + j] = value; // 比对称矩阵多了0号公共常量，所以下标加1。
    }
    int Get(int i, int j) const {
        CheckPosition(n, i, j);
        return i < j ? a[0] : a[i * (i - 1) / 2 + j];
    }
    void ShowStorage() const { for (int x : a) cout << x << ' '; cout << '\n'; }
};

class TriDiagonalMatrix {
    int n;
    vector<int> a;
public:
    explicit TriDiagonalMatrix(int order, int common = 0)
        : n(ValidOrder(order)), a(3 * n - 1, 0) { a[0] = common; }
    void Set(int i, int j, int value) {
        CheckPosition(n, i, j);
        if (abs(i - j) > 1) {
            if (value != a[0]) throw invalid_argument("带外元素只能取公共常量c");
            return;
        }
        // 三条对角线共有3n-2项；另加一个公共常量，总长3n-1。
        a[2 * (i - 1) + j] = value;
    }
    int Get(int i, int j) const {
        CheckPosition(n, i, j);
        return abs(i - j) > 1 ? a[0] : a[2 * (i - 1) + j];
    }
    void ShowStorage() const { for (int x : a) cout << x << ' '; cout << '\n'; }
};

int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    SymmetricMatrix s(3);
    const int sym[3][3] = {{1,2,3},{2,4,5},{3,5,6}};
    for (int i = 1; i <= 3; ++i)
        for (int j = 1; j <= i; ++j) s.Set(i, j, sym[i - 1][j - 1]);
    cout << "对称矩阵压缩数组："; s.ShowStorage();
    cout << "S(1,3)=" << s.Get(1, 3) << '\n';

    LowerTriangularMatrix l(3);
    const int low[3][3] = {{1,0,0},{2,3,0},{4,5,6}};
    for (int i = 1; i <= 3; ++i)
        for (int j = 1; j <= i; ++j) l.Set(i, j, low[i - 1][j - 1]);
    cout << "下三角压缩数组（首项为c）："; l.ShowStorage();
    cout << "L(1,3)=" << l.Get(1, 3) << '\n';

    TriDiagonalMatrix t(4);
    const int tri[4][4] = {{1,2,0,0},{3,4,5,0},{0,6,7,8},{0,0,9,10}};
    for (int i = 1; i <= 4; ++i)
        for (int j = 1; j <= 4; ++j) t.Set(i, j, tri[i - 1][j - 1]);
    cout << "三对角压缩数组（首项为c）："; t.ShowStorage();
    cout << "T(3,4)=" << t.Get(3, 4) << ", T(1,4)=" << t.Get(1, 4) << '\n';

    int kind = 0, i = 0, j = 0;
    cout << "查询类型：1对称 2下三角 3三对角；0结束：";
    if (!(cin >> kind)) {
        if (cin.eof()) return 0;
        cout << "错误：查询类型须为整数。\n"; return 1;
    }
    if (kind == 0) return 0;
    cout << "输入行号和列号（从1开始）：";
    if (!(cin >> i >> j)) { cout << "错误：请输入两个整数。\n"; return 1; }
    try {
        int value;
        if (kind == 1) value = s.Get(i, j);
        else if (kind == 2) value = l.Get(i, j);
        else if (kind == 3) value = t.Get(i, j);
        else throw invalid_argument("查询类型不合法");
        cout << "查询结果：" << value << '\n';
    } catch (const exception& e) { cout << "错误：" << e.what() << '\n'; return 1; }
    return 0;
}
