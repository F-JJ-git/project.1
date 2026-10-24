/*
 * 课件第21～24页：完全二叉树定义、1基层序编号及顺序存储性质。
 * 输入：总结点数n和待查询编号i，要求n>0、1<=i<=n。
 * 输出：双亲、左孩子、右孩子编号或不存在的提示；时间/额外空间均O(1)。
 * 公式：双亲i/2，左孩子2*i，右孩子2*i+1；这些是完全二叉树编号，不适用于任意标签。
 */
#include <iostream>
#include <limits>
#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif
using namespace std;

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    long long n = 0, i = 0;
    cout << "输入完全二叉树总结点数 n：";
    if (!(cin >> n)) { cout << "输入格式错误。\n"; return 1; }
    cout << "输入要查询的结点编号 i：";
    if (!(cin >> i)) { cout << "输入格式错误。\n"; return 1; }
    if (n <= 0 || i < 1 || i > n) {
        cout << "输入不合法：要求 n>0 且 1<=i<=n。\n"; return 1;
    }
    if (i == 1) cout << "该结点是根结点，没有双亲。\n";
    else cout << "双亲编号：" << i / 2 << '\n';

    // 先用除法检查孩子是否存在，再乘2，避免查询很大编号时溢出。
    if (i <= n / 2) cout << "左孩子编号：" << 2 * i << '\n';
    else cout << "没有左孩子。\n";
    if (i <= (n - 1) / 2) cout << "右孩子编号：" << 2 * i + 1 << '\n';
    else cout << "没有右孩子。\n";
    return 0;
}
