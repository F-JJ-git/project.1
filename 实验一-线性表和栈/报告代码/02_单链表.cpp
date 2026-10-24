#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <stdexcept>
#include <cmath>
#include <cctype>
#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif
using namespace std;
#define MAXSIZE 1000

void InitConsole()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
}

struct Book
{
    string ISBN;       // ISBN
    string publisher;  // 出版社
    string author;     // 作者
    double price;      // 价格
};
// 带头结点单链表；禁止浅复制，析构时释放所有结点。
class LinkList {
    struct Node { Book data; Node* next; };
    Node* head = new Node{Book{}, nullptr};
    int count = 0;
public:
    LinkList() = default;
    LinkList(const LinkList&) = delete;
    LinkList& operator=(const LinkList&) = delete;
    ~LinkList() { clear(); delete head; }
    int size() const { return count; }
    void clear() {
        while (head->next) {
            Node* q = head->next;
            head->next = q->next;
            delete q;
        }
        count = 0;
    }
    void create(const vector<Book>& books) {
        clear(); Node* tail = head;
        for (const Book& b : books) {
            tail->next = new Node{b, nullptr};
            tail = tail->next; ++count;
        }
    }
    bool get(int i, Book& b) const {
        if (i < 1 || i > count) return false;
        Node* p = head->next;
        for (int j = 1; j < i; ++j) p = p->next;
        b = p->data; return true;
    }
    int locate(const string& isbn) const {
        int i = 1;
        for (Node* p = head->next; p; p = p->next, ++i)
            if (p->data.ISBN == isbn) return i;
        return 0;
    }
    bool insert(int i, const Book& b) {
        if (i < 1 || i > count + 1) return false;
        Node* p = head;
        for (int j = 1; j < i; ++j) p = p->next;
        p->next = new Node{b, p->next}; ++count;
        return true;
    }
    bool erase(int i) {
        if (i < 1 || i > count) return false;
        Node* p = head;
        for (int j = 1; j < i; ++j) p = p->next;
        Node* q = p->next;
        p->next = q->next; delete q; --count;
        return true;
    }
    bool sorted() const {
        for (Node* p = head->next; p && p->next; p = p->next)
            if (p->data.ISBN > p->next->data.ISBN) return false;
        return true;
    }
    // 复制式归并保留输入表；拒绝输出与输入别名和无序输入。
    static bool merge(const LinkList& a, const LinkList& b, LinkList& out) {
        if (&out == &a || &out == &b || !a.sorted() || !b.sorted())
            return false;
        out.clear(); Node* tail = out.head;
        Node* p = a.head->next; Node* q = b.head->next;
        while (p || q) {
            Node*& chosen = (!q || (p && p->data.ISBN <= q->data.ISBN)) ? p : q;
            tail->next = new Node{chosen->data, nullptr};
            tail = tail->next; chosen = chosen->next; ++out.count;
        }
        return true;
    }
    vector<Book> values() const {
        vector<Book> result;
        for (Node* p = head->next; p; p = p->next) result.push_back(p->data);
        return result;
    }
};

void PrintBook(const Book& b)
{
    cout << b.ISBN << " | " << b.publisher << " | "
         << b.author << " | " << b.price << endl;
}


template<class T> T ReadNumber(const string& prompt)
{
    while (true) {
        cout << prompt;
        string line; T value; char extra;
        if (!getline(cin, line)) throw runtime_error("输入结束");
        istringstream in(line);
        if ((in >> value) && !(in >> extra)) return value;
        cout << "输入格式错误，请重新输入。" << endl;
    }
}

Book ReadBook()
{
    Book b;
    do {
        cout << "ISBN：";
        if (!getline(cin, b.ISBN)) throw runtime_error("输入结束");
    } while (b.ISBN.empty());
    cout << "出版社：";
    if (!getline(cin, b.publisher)) throw runtime_error("输入结束");
    cout << "作者：";
    if (!getline(cin, b.author)) throw runtime_error("输入结束");
    do {
        b.price = ReadNumber<double>("价格（非负数）：");
    } while (!isfinite(b.price) || b.price < 0);
    return b;
}

int main()
{
    InitConsole();
    LinkList L;
    try {
        while (true) {
            cout << "\n单链表：1初始化 2建表 3取值 4查找" << endl;
            cout << "5插入 6删除 7归并 8输出 9清空 0退出" << endl;
            int op = ReadNumber<int>("请选择：");
            if (op == 0) break;
            if (op == 1 || op == 9) {
                L.clear(); cout << "空表已就绪，表长=0" << endl;
            } else if (op == 2 || op == 7) {
                int n = ReadNumber<int>("教材数量：");
                if (n < 0) { cout << "数量不能为负。" << endl; continue; }
                vector<Book> books;
                for (int i = 0; i < n; ++i) {
                    cout << "第" << i + 1 << "本教材" << endl;
                    books.push_back(ReadBook());
                }
                if (op == 2) {
                    L.create(books); cout << "建表完成，表长=" << L.size() << endl;
                } else {
                    LinkList A, B; A.create(books);
                    if (!LinkList::merge(L, A, B))
                        cout << "归并失败：输入表未按ISBN非递减排列。" << endl;
                    else {
                        cout << "归并结果：" << endl;
                        for (const Book& b : B.values()) PrintBook(b);
                        cout << "归并后表长=" << B.size() << endl;
                    }
                }
            } else if (op == 3) {
                Book b; int i = ReadNumber<int>("取值位序：");
                if (L.get(i, b)) PrintBook(b);
                else cout << "位序不合法。" << endl;
            } else if (op == 4) {
                string isbn; cout << "查找ISBN：";
                if (!getline(cin, isbn)) break;
                cout << "位序=" << L.locate(isbn) << "（0表示未找到）" << endl;
            } else if (op == 5) {
                int i = ReadNumber<int>("插入位序：");
                if (i < 1 || i > L.size() + 1) { cout << "位序不合法。" << endl; continue; }
                Book b = ReadBook(); L.insert(i, b);
                cout << "插入成功，表长=" << L.size() << endl;
            } else if (op == 6) {
                int i = ReadNumber<int>("删除位序：");
                cout << (L.erase(i) ? "删除成功。" : "位序不合法。") << endl;
            } else if (op == 8) {
                if (!L.size()) cout << "链表为空。" << endl;
                for (const Book& b : L.values()) PrintBook(b);
            } else cout << "无效选项。" << endl;
        }
    } catch (const exception& e) { cout << e.what() << endl; }
    return 0;
}
