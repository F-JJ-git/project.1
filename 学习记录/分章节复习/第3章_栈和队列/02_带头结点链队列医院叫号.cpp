/*
带头结点链队列：医院取号和叫号
课件：第3章第30—42张（FIFO链队列）、第106张作业3.2。
输入菜单1取号、2叫号、3查看、0退出；相对作业增加查看菜单，退出编号改为0。
front固定指向哨兵头结点，rear指向末数据结点；队空时front==rear。
入队尾插、出队头删，均为O(1)；显示与释放O(n)。未设置固定队列容量，受可用内存限制。
*/

#include <iostream>
#include <sstream>
#include <string>
#include <limits>
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

struct TicketNode {
    int number;
    TicketNode* next;
    explicit TicketNode(int n = 0, TicketNode* p = nullptr) : number(n), next(p) {}
};

class TicketQueue {
private:
    TicketNode* front; // 指向头结点
    TicketNode* rear;  // 指向最后一个有效结点；空队列时 rear == front
    int nextNumber;
public:
    TicketQueue() : front(new TicketNode()), rear(front), nextNumber(1) {}
    ~TicketQueue() {
        while (front) { TicketNode* p = front; front = front->next; delete p; }
    }
    TicketQueue(const TicketQueue&) = delete;
    TicketQueue& operator=(const TicketQueue&) = delete;

    int TakeNumber() {
        if (nextNumber == numeric_limits<int>::max()) throw overflow_error("候诊号已达上限");
        int num = nextNumber;
        TicketNode* p = new TicketNode(num); // 分配成功后才消耗号码。
        ++nextNumber;
        rear->next = p;
        rear = p;
        return num;
    }
    bool CallNumber(int& num) {
        if (front == rear) return false;
        TicketNode* p = front->next;
        num = p->number;
        front->next = p->next;
        // 删除唯一数据结点后，rear必须重新指向头结点，不能留下悬空指针。
        if (rear == p) rear = front;
        delete p;
        return true;
    }
    void Display() const {
        if (front == rear) { cout << "当前没有候诊患者。\n"; return; }
        cout << "候诊队列：";
        for (TicketNode* p = front->next; p; p = p->next) cout << p->number << (p->next ? " -> " : "\n");
    }
};

int main() {
    InitConsole();
    TicketQueue q;
    string line;
    while (true) {
        cout << "\n1.取号  2.叫号  3.查看队列  0.退出\n请选择：";
        if (!getline(cin, line)) break;
        istringstream input(line);
        int choice = 0; char extra = 0;
        if (!(input >> choice) || (input >> extra)) { cout << "输入错误，请输入菜单整数。\n"; continue; }
        if (choice == 0) break;
        try {
            if (choice == 1) cout << "您的候诊号是：" << q.TakeNumber() << '\n';
            else if (choice == 2) {
                int num = 0;
                if (q.CallNumber(num)) cout << "请 " << num << " 号患者就诊。\n";
                else cout << "当前没有候诊患者。\n";
            } else if (choice == 3) q.Display();
            else cout << "无效选项。\n";
        } catch (const exception& e) { cout << "错误：" << e.what() << '\n'; }
    }
}
