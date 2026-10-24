/*
两个非递减有序线性表归并
课件：第2章第52—54张幻灯片，示例La=(2,5,8,11)，Lb=(2,3,9,11,15,20)。
输入：1课件示例；2空表/重复值/无序输入。输出新的非递减表，重复值保留。
本版用vector顺序访问实现双指针；课件用SimpleLinkList的按位接口表达相同选择过程。
相等时本版先取a（课件先取b），整数结果一致；输入表均保留。时间/结果空间O(n+m)。
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif

void InitConsole() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
}

using namespace std;

vector<int> MergeSorted(const vector<int>& a, const vector<int>& b) {
    if (!is_sorted(a.begin(), a.end()) || !is_sorted(b.begin(), b.end()))
        throw invalid_argument("输入表必须按非递减顺序排列");
    vector<int> c;
    c.reserve(a.size() + b.size());
    size_t i = 0, j = 0;
    // 不变量：c已经有序，a[0..i)、b[0..j)已处理；每轮只前进一个下标。
    while (i < a.size() && j < b.size()) {
        if (a[i] <= b[j]) c.push_back(a[i++]);
        else c.push_back(b[j++]);
    }
    // 一方用尽后，另一方剩余元素已经有序，直接追加。
    while (i < a.size()) c.push_back(a[i++]);
    while (j < b.size()) c.push_back(b[j++]);
    return c;
}

void Print(const vector<int>& a) {
    cout << "(";
    for (size_t i = 0; i < a.size(); ++i) cout << (i ? ", " : "") << a[i];
    cout << ")\n";
}

int main() {
    InitConsole();
    int mode = 1;
    cout << "1.课件示例  2.边界检查（默认1）：";
    if (!(cin >> mode)) mode = 1;
    if (mode == 2) {
        cout << "空表与(1,1)归并："; Print(MergeSorted({}, {1, 1}));
        cout << "两个空表归并："; Print(MergeSorted({}, {}));
        try { Print(MergeSorted({2, 1}, {3})); }
        catch (const invalid_argument& e) { cout << "错误：" << e.what() << '\n'; }
        return 0;
    }
    vector<int> a{2, 5, 8, 11}, b{2, 3, 9, 11, 15, 20};
    cout << "La："; Print(a);
    cout << "Lb："; Print(b);
    cout << "归并后的Lc："; Print(MergeSorted(a, b));
}
