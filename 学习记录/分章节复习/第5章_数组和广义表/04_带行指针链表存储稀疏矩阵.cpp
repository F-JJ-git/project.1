// 第5章：带行指针的链表存储稀疏矩阵。
// 课件：幻灯片37、48。此结构每行一个单链表，不是49页标注“不讲”的十字链表。
// 每行按列号升序，只保存非零元素；行号由rowHead的位置隐含表示。
// 输入：演示后可输入 r c value 设置一个元素；EOF结束。行列号从1开始。
// 第r行有t_r项时，Get/Set为O(t_r)，修改已定位结点为O(1)。空间O(rows+t)。
#include <iostream>
#include <vector>
#include <stdexcept>
#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif
using namespace std;

struct Node {
    int col, value;
    Node* next;
    Node(int c, int v, Node* p = nullptr) : col(c), value(v), next(p) {}
};

int ValidSize(int n)
{
    if (n < 1 || n > 1000) throw invalid_argument("示例行列数须在1至1000之间");
    return n;
}

class RowPointerSparseMatrix {
    int rows, cols;
    vector<Node*> rowHead;
public:
    RowPointerSparseMatrix(int r, int c)
        : rows(ValidSize(r)), cols(ValidSize(c)), rowHead(rows, nullptr) {}
    ~RowPointerSparseMatrix() {
        for (Node* head : rowHead)
            while (head) { Node* p = head; head = head->next; delete p; }
    }
    // 含拥有权的裸指针不能默认浅拷贝；本练习禁止复制，避免重复释放结点。
    RowPointerSparseMatrix(const RowPointerSparseMatrix&) = delete;
    RowPointerSparseMatrix& operator=(const RowPointerSparseMatrix&) = delete;

    bool SetElem(int r, int c, int value) {
        if (r < 1 || r > rows || c < 1 || c > cols) return false;
        Node*& head = rowHead[r - 1];
        Node* prev = nullptr;
        Node* cur = head;
        // 循环结束时，prev位于目标之前，cur为第一个列号不小于c的结点。
        while (cur && cur->col < c) { prev = cur; cur = cur->next; }
        if (cur && cur->col == c) {
            if (value == 0) {
                if (prev) prev->next = cur->next;
                else head = cur->next; // 删除本行首个结点时必须修改行头指针。
                delete cur;
            } else cur->value = value;
        } else if (value != 0) {
            Node* p = new Node(c, value, cur);
            if (prev) prev->next = p;
            else head = p;
        }
        return true;
    }
    int GetElem(int r, int c) const {
        if (r < 1 || r > rows || c < 1 || c > cols) throw out_of_range("下标越界");
        for (Node* p = rowHead[r - 1]; p; p = p->next) {
            if (p->col == c) return p->value;
            if (p->col > c) break; // 有序链表可提前结束查找。
        }
        return 0;
    }
    void Display() const {
        for (int r = 0; r < rows; ++r) {
            cout << "第" << r + 1 << "行：";
            if (!rowHead[r]) cout << "无非零元";
            for (Node* p = rowHead[r]; p; p = p->next)
                cout << " (" << r + 1 << ',' << p->col << ',' << p->value << ')';
            cout << '\n';
        }
    }
};

int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    RowPointerSparseMatrix a(4,5);
    a.SetElem(1,1,3); a.SetElem(1,4,8);
    a.SetElem(2,2,5); a.SetElem(3,1,7); a.SetElem(3,5,9);
    cout << "带行指针链表：\n"; a.Display();
    cout << "A(3,5)=" << a.GetElem(3,5) << '\n';
    a.SetElem(3,5,0);
    cout << "删除A(3,5)后：\n"; a.Display();
    cout << "可输入 行 列 值 设置A（4行5列；EOF结束）：";
    int r = 0, c = 0, value = 0;
    if (!(cin >> r)) {
        if (cin.eof()) return 0;
        cout << "错误：行号须为整数。\n"; return 1;
    }
    if (!(cin >> c >> value)) { cout << "错误：请输入三个整数。\n"; return 1; }
    if (!a.SetElem(r,c,value)) { cout << "设置失败：下标越界。\n"; return 1; }
    cout << "设置后A(" << r << ',' << c << ")=" << a.GetElem(r,c) << '\n';
    a.Display();
    return 0;
}
