/*
循环链表模拟约瑟夫问题
课件：第2章第60—66张幻灯片；第65张示例n=8、m=3。
输入：正整数n、m。输出n-1人的出列顺序和最后一位优胜者。
本版采用第61张不带头结点的圆环，直接保存当前结点和前驱；末尾释放幸存结点。
取模后时间O(n + Σ((m-1) % r))，最坏O(n²)，空间O(n)；人数10000为演示输入限制。
*/

#include <iostream>

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

struct Node {
    int id;
    Node* next;
    explicit Node(int x) : id(x), next(nullptr) {}
};

void Josephus(int n, long long m) {
    if (n <= 0 || m <= 0) { cout << "n 和 m 必须为正数。\n"; return; }
    Node* head = new Node(1);
    Node* tail = head;
    for (int i = 2; i <= n; ++i) { tail->next = new Node(i); tail = tail->next; }
    tail->next = head; // 首尾相连，形成循环链表

    // prev始终是cur的前驱；从1号开始报1，走m-1步后删除cur。
    Node* prev = tail;
    int remaining = n;
    Node* cur = head;
    cout << "出列顺序：";
    while (cur->next != cur) {
        // 多走完整一圈不会改变目标，取模避免m很大时做无用循环。
        const long long steps = (m - 1) % remaining;
        for (long long step = 0; step < steps; ++step) { prev = cur; cur = cur->next; }
        cout << cur->id << ' ';
        prev->next = cur->next;
        delete cur;
        cur = prev->next; // 出列者的下一人重新报1。
        --remaining;
    }
    cout << "\n优胜者：" << cur->id << '\n';
    delete cur;
}

int main() {
    InitConsole();
    int n = 0; long long m = 0;
    cout << "输入人数n和报数m（例如8 3；人数最多10000）：";
    if (!(cin >> n >> m)) { cout << "输入错误：请输入两个整数。\n"; return 1; }
    if (n > 10000) { cout << "演示人数不能超过10000。\n"; return 1; }
    Josephus(n, m);
}
