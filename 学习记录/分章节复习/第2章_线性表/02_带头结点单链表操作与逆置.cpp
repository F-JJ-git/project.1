/*
带头结点单链表与头插法就地逆置
课件：第2章第34—51、55—59张幻灯片；本版直接按57—59张调整指针。
输入：1运行正常操作；2运行空表、单结点、非法位序检查。输出链表和长度。
不变量：head不存有效元素，末结点next为空；逆置时cur指向待处理部分，head->next指向已逆置部分。
复杂度：Length/按位插入/删除O(n)；已找到前驱后改链接O(1)；逆置O(n)，额外空间O(1)。
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
    int data;
    Node* next;
    explicit Node(int value = 0, Node* link = nullptr) : data(value), next(link) {}
};

class LinkList {
private:
    Node* head; // 头指针指向头结点；头结点不存有效元素
public:
    LinkList() : head(new Node()) {}
    ~LinkList() { Clear(); delete head; }
    LinkList(const LinkList&) = delete;
    LinkList& operator=(const LinkList&) = delete;

    int Length() const {
        int n = 0;
        for (Node* p = head->next; p; p = p->next) ++n;
        return n;
    }
    bool Insert(int position, int e) { // 在第 position 个位置前插入
        if (position < 1 || position > Length() + 1) return false;
        // 先找到前驱，找到后改变链接本身只需O(1)。
        Node* prev = head;
        for (int i = 1; i < position; ++i) prev = prev->next;
        prev->next = new Node(e, prev->next);
        return true;
    }
    bool Delete(int position, int& e) {
        if (position < 1 || position > Length()) return false;
        // 先找到前驱，找到后改变链接本身只需O(1)。
        Node* prev = head;
        for (int i = 1; i < position; ++i) prev = prev->next;
        Node* victim = prev->next;
        e = victim->data;
        prev->next = victim->next;
        delete victim;
        return true;
    }
    void Reverse() {
        // 对照课件57—59张：将尚未处理的结点逐个摘下，插到头结点之后。
        Node* cur = head->next;
        head->next = nullptr; // 已逆置部分初始为空；头结点始终保留。
        while (cur) {
            Node* next = cur->next; // 改next以前先保存剩余链，避免丢链。
            cur->next = head->next;
            head->next = cur;
            cur = next;
        }
    }
    void Clear() {
        Node* p = head->next;
        while (p) { Node* next = p->next; delete p; p = next; }
        head->next = nullptr;
    }
    void Display() const {
        for (Node* p = head->next; p; p = p->next) cout << p->data << (p->next ? " -> " : "\n");
        if (!head->next) cout << "空表\n";
    }
};

int main() {
    InitConsole();
    int mode = 1;
    cout << "1.课件示例  2.边界检查（默认1）：";
    if (!(cin >> mode)) mode = 1;
    LinkList list;
    int removed = 99;
    if (mode == 2) {
        list.Reverse(); cout << "空表逆置后："; list.Display();
        cout << "空表删除：" << list.Delete(1, removed) << "，输出参数=" << removed << '\n';
        cout << "非法插入：" << list.Insert(0, 5) << '\n';
        list.Insert(1, 10); list.Reverse(); list.Delete(1, removed);
        cout << "删除唯一元素：" << removed << "，长度=" << list.Length() << '\n';
        list.Display(); return 0;
    }
    for (int x : {10, 20, 30, 40}) list.Insert(list.Length() + 1, x);
    cout << "初始链表："; list.Display();
    list.Insert(3, 25); cout << "在第3位插入25："; list.Display();
    if (list.Delete(2, removed)) cout << "删除的元素：" << removed << '\n';
    cout << "删除后："; list.Display();
    list.Reverse(); cout << "逆置后："; list.Display();
    cout << "链表长度：" << list.Length() << '\n';
}
