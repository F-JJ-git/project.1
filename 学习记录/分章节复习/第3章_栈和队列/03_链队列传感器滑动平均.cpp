/*
链队列应用练习：最近n个传感器数据的滑动平均
课件依据：第3章第30—42张的先进先出与链队列；滑动平均是以此扩展的应用，非课件逐字例题。
输入：正窗口容量n、非负采集次数m，随后m个有限实数。输出每次平均值与当前窗口。
窗口未满时除以实际count；满时先淘汰最早数据。维护sum使均值更新O(1)，显示窗口O(n)。
课件里的普通队列不会自动淘汰，本例的“满时删旧值”是滑动窗口业务规则。
*/

#include <iostream>
#include <cmath>
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

struct Node {
    double data;
    Node* next;
    explicit Node(double e = 0, Node* p = nullptr) : data(e), next(p) {}
};

class SensorQueue {
private:
    Node* front; // 头结点，不保存有效数据
    Node* rear;
    int maxSize;
    int count;
    double sum;
public:
    explicit SensorQueue(int n) : front(nullptr), rear(nullptr), maxSize(n), count(0), sum(0) {
        if (n <= 0) throw invalid_argument("容量必须大于0");
        rear = front = new Node();
    }
    ~SensorQueue() {
        while (front) { Node* p = front; front = front->next; delete p; }
    }
    SensorQueue(const SensorQueue&) = delete;
    SensorQueue& operator=(const SensorQueue&) = delete;

    double AddData(double e) {
        if (!isfinite(e)) throw invalid_argument("采集值必须为有限数");
        const double expired = (count == maxSize) ? front->next->data : 0.0;
        const double nextSum = sum - expired + e;
        if (!isfinite(nextSum)) throw overflow_error("窗口总和超出double范围");
        Node* p = new Node(e); // 先申请新结点，申请失败时原窗口仍完整。
        // 不变量：链表保存最近count个值，sum恰是这些值的总和。
        if (count == maxSize) { // 满时先删除最早采集的数据
            Node* old = front->next;
            front->next = old->next;
            if (rear == old) rear = front;
            delete old;
            --count;
        }
        rear->next = p;
        rear = p;
        sum = nextSum;
        ++count;
        return sum / count;
    }
    void Display() const {
        cout << "当前窗口：";
        for (Node* p = front->next; p; p = p->next) cout << p->data << (p->next ? " -> " : "");
        cout << "\n";
    }
};

int main() {
    InitConsole();
    int n = 0, m = 0;
    cout << "输入数据区容量n和采集次数m：";
    if (!(cin >> n >> m)) { cout << "输入错误：请输入两个整数。\n"; return 1; }
    if (n <= 0) { cout << "容量必须大于0。\n"; return 1; }
    if (m < 0) { cout << "次数不能为负数。\n"; return 1; }
    try {
        SensorQueue q(n);
        for (int i = 0; i < m; ++i) {
            double e = 0;
            cout << "输入新采集的数据：";
            if (!(cin >> e)) { cout << "输入错误：采集值必须为有限数。\n"; return 1; }
            double average = q.AddData(e);
            cout << "当前平均值：" << average << '\n';
            q.Display();
        }
    } catch (const exception& e) { cout << "错误：" << e.what() << '\n'; return 1; }
}
