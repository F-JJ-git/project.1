// 第5章：寻找方阵的所有马鞍点。
// 课件：幻灯片76，作业5.3。定义为“所在行最小且所在列最大”。
// 输入：n(1至300)，随后按行输入n*n个整数；输出位置使用1基行列号。
// 先计算各行最小值、各列最大值，再检查所有位置，避免漏掉并列最值。
// 时间O(n^2)，矩阵空间O(n^2)，两个最值数组的辅助空间O(n)。
#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif
using namespace std;

int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    int n = 0;
    cout << "输入方阵阶数n（1至300）：";
    if (!(cin >> n) || n < 1 || n > 300) {
        cout << "错误：n须为1至300的整数。\n";
        return 1;
    }
    vector<vector<int>> a(n, vector<int>(n));
    vector<int> rowMin(n, numeric_limits<int>::max());
    vector<int> colMax(n, numeric_limits<int>::min());
    cout << "按行输入矩阵元素：\n";
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j) {
            if (!(cin >> a[i][j])) { cout << "错误：矩阵元素必须为整数且数量完整。\n"; return 1; }
            rowMin[i] = min(rowMin[i], a[i][j]);
            colMax[j] = max(colMax[j], a[i][j]);
        }

    int count = 0;
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            // 不能每行只选一个最小值；相等的多个位置都可能是马鞍点。
            if (a[i][j] == rowMin[i] && a[i][j] == colMax[j]) {
                cout << "马鞍点：a(" << i + 1 << ',' << j + 1 << ") = " << a[i][j] << '\n';
                ++count;
            }
    if (count == 0) cout << "矩阵中没有马鞍点。\n";
    cout << "马鞍点总数：" << count << '\n';
    return 0;
}
