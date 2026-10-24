/*
带头结点循环双向链表
课件：第2章第67—72张幻灯片；本版省略curPtr/curPosition缓存，直接遍历找位置。
输入：1正常操作；2空表、单结点与非法位序。输出正向和反向序列。
不变量：空表head->next==head->prev==head；每条链接满足p->next->prev==p。
按位操作含查找为O(n)，已知结点后插删链接O(1)，完整遍历和析构O(n)。
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

struct DNode {
    int data;
    DNode* prev;
    DNode* next;
    explicit DNode(int x = 0) : data(x), prev(this), next(this) {}
};

class CircularDoublyList {
private:
    DNode* head; // 哨兵头结点，不存有效数据
public:
    CircularDoublyList() : head(new DNode()) {}
    ~CircularDoublyList() {
        DNode* p = head->next;
        while (p != head) { DNode* next = p->next; delete p; p = next; }
        delete head;
    }
    CircularDoublyList(const CircularDoublyList&) = delete;
    CircularDoublyList& operator=(const CircularDoublyList&) = delete;

    int Length() const {
        int n = 0;
        for (DNode* p = head->next; p != head; p = p->next) ++n;
        return n;
    }
    bool Insert(int position, int e) { // 在第 position 个位置前插入，position 可为 Length()+1
        int len = Length();
        if (position < 1 || position > len + 1) return false;
        // 插到position前：position=len+1时后继就是head，统一处理尾插。
        DNode* nextNode = head;
        for (int i = 0; i < position; ++i) nextNode = nextNode->next;
        DNode* prevNode = nextNode->prev;
        DNode* p = new DNode(e);
        // 先连好新结点的两条指针，再让前驱和后继接纳它。
        p->prev = prevNode; p->next = nextNode;
        prevNode->next = p; nextNode->prev = p;
        return true;
    }
    bool Delete(int position, int& e) {
        int len = Length();
        if (position < 1 || position > len) return false;
        DNode* p = head->next;
        for (int i = 1; i < position; ++i) p = p->next;
        e = p->data;
        // 两个方向都要旁路，漏改任意一条都会破坏逆向/正向遍历。
        p->prev->next = p->next;
        p->next->prev = p->prev;
        delete p;
        return true;
    }
    void DisplayForward() const {
        cout << "正向：";
        for (DNode* p = head->next; p != head; p = p->next) cout << p->data << ' ';
        cout << '\n';
    }
    void DisplayBackward() const {
        cout << "反向：";
        for (DNode* p = head->prev; p != head; p = p->prev) cout << p->data << ' ';
        cout << '\n';
    }
};

int main() {
    InitConsole();
    int mode = 1;
    cout << "1.课件示例  2.边界检查（默认1）：";
    if (!(cin >> mode)) mode = 1;
    CircularDoublyList list;
    int e = 99;
    if (mode == 2) {
        cout << "空表删除：" << list.Delete(1, e) << "，输出参数=" << e << '\n';
        cout << "非法位序插入：" << list.Insert(2, 10) << '\n';
        list.Insert(1, 10); list.Delete(1, e);
        cout << "删除唯一结点：" << e << "，长度=" << list.Length() << '\n';
        list.DisplayForward(); list.DisplayBackward(); return 0;
    }
    for (int x : {10, 20, 30}) list.Insert(list.Length() + 1, x);
    list.DisplayForward(); list.DisplayBackward();
    list.Insert(2, 15);
    cout << "第2位插入15后：\n"; list.DisplayForward(); list.DisplayBackward();
    if (list.Delete(3, e)) cout << "删除第3位元素：" << e << '\n';
    list.DisplayForward(); list.DisplayBackward();
}
