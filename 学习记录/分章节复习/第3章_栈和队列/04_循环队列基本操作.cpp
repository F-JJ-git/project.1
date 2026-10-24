/*
循环队列的计数法实现
课件：第3章第43—55张，重点第49—51张区分空一法、计数法、标记法。
输入：1满队列与回绕示例；2空队列、容量1、清空和非法容量。输出队列及操作结果。
不变量：0<=count<=容量，rear==(front+count)%容量；队空和队满时都可能front==rear。
本版用count判空/满且遍历count次，修正课件第53张遍历片段在满队列下遗漏元素的问题。
入队、出队、取头、长度O(1)，遍历O(count)，数组空间O(容量)，不预留空位。
*/

#include <iostream>
#include <vector>
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

class CircularQueue {
private:
    vector<int> data;
    int front, rear, count;
public:
    explicit CircularQueue(int capacity) : front(0), rear(0), count(0) {
        if (capacity <= 0) throw invalid_argument("容量必须大于0");
        data.resize(static_cast<size_t>(capacity));
    }
    int Length() const { return count; }
    void Clear() { front = rear = count = 0; }
    bool GetHead(int& e) const {
        if (Empty()) return false;
        e = data[front]; return true;
    }
    bool Empty() const { return count == 0; }
    bool Full() const { return count == static_cast<int>(data.size()); }
    bool EnQueue(int e) {
        if (Full()) return false;
        // rear表示下一次写入位置，不是当前最后一个有效元素的位置。
        data[rear] = e;
        rear = (rear + 1) % static_cast<int>(data.size());
        ++count;
        return true;
    }
    bool DeQueue(int& e) {
        if (Empty()) return false;
        e = data[front];
        front = (front + 1) % static_cast<int>(data.size());
        --count;
        return true;
    }
    void Display() const {
        cout << "队列：";
        // 计数法必须遍历count次；满队列front==rear，不能以指针相等提前结束。
        for (int i = 0; i < count; ++i) cout << data[(front + i) % data.size()] << ' ';
        cout << '\n';
    }
};

int main() {
    InitConsole();
    int mode = 1;
    cout << "1.回绕示例  2.边界检查（默认1）：";
    if (!(cin >> mode)) mode = 1;
    int e = 99;
    if (mode == 2) {
        CircularQueue q(1);
        cout << "空队列出队：" << q.DeQueue(e) << "，输出参数=" << e << '\n';
        cout << "空队列取头：" << q.GetHead(e) << '\n';
        q.EnQueue(7); cout << "容量1已满：" << q.Full() << '\n'; q.Display();
        q.DeQueue(e); q.EnQueue(8); q.Clear();
        cout << "清空后长度：" << q.Length() << '\n';
        try { CircularQueue invalid(0); }
        catch (const invalid_argument& error) { cout << "错误：" << error.what() << '\n'; }
        return 0;
    }
    CircularQueue q(3);
    q.EnQueue(10); q.EnQueue(20); q.EnQueue(30); q.Display();
    cout << "尝试入队40：" << (q.EnQueue(40) ? "成功" : "队满") << '\n';
    if (q.DeQueue(e)) cout << "出队：" << e << '\n';
    q.EnQueue(40); q.Display();
    if (q.GetHead(e)) cout << "队头：" << e << "，长度：" << q.Length() << '\n';
}
