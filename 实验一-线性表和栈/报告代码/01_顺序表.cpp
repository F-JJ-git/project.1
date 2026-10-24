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
struct SqList
{
    Book* elem;     // 教材数组的首地址
    int length;     // 当前教材数量
};

// 1. 初始化
void InitList(SqList& L)
{
    L.elem = new Book[MAXSIZE];
    L.length = 0;
}

// 3. 取值
bool GetElem(SqList L, int i, Book& e)
{
    if (i > L.length || i < 1)
    {
        return false;
    }

    e = L.elem[i - 1];
    return true;
}

// 4. 查找
int LocateElem(SqList L, string ISBN)
{
    for (int i = 0; i < L.length; i++)
    {
        if (L.elem[i].ISBN == ISBN)
        {
            return i + 1;
        }
    }

    return 0;
}

// 5. 插入
bool ListInsert(SqList& L, int i, Book e)
{
    if (i < 1 || i > L.length + 1 || L.length == MAXSIZE)
    {
        return false;
    }

    for (int j = L.length - 1; j >= i - 1; j--)
    {
        L.elem[j + 1] = L.elem[j];
    }

    L.elem[i - 1] = e;
    L.length++;

    return true;
}

// 6. 删除
bool ListDelete(SqList& L, int i)
{
    if (i < 1 || i > L.length)
    {
        return false;
    }

    for (int j = i - 1; j < L.length - 1; j++)
    {
        L.elem[j] = L.elem[j + 1];
    }

    L.length--;
    return true;
}

// 7. 归并
bool MergeList(SqList L, SqList A, SqList& B)
{
    if (L.length + A.length > MAXSIZE)
    {
        return false;
    }

    int i = 0, j = 0, k = 0;

    while (i < L.length && j < A.length)
    {
        if (L.elem[i].ISBN <= A.elem[j].ISBN)
        {
            B.elem[k++] = L.elem[i++];
        }
        else
        {
            B.elem[k++] = A.elem[j++];
        }
    }

    while (i < L.length)
    {
        B.elem[k++] = L.elem[i++];
    }

    while (j < A.length)
    {
        B.elem[k++] = A.elem[j++];
    }

    B.length = k;
    return true;
}

// 9. 清空
bool ClearList(SqList& L)
{
    if (L.elem != nullptr)
    {
        delete[] L.elem;
        L.elem = nullptr;
    }

    L.length = 0;
    return true;
}

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

// 有序归并的输入必须按ISBN字典序非递减排列。
bool IsSorted(const SqList& L)
{
    for (int i = 1; i < L.length; ++i)
        if (L.elem[i - 1].ISBN > L.elem[i].ISBN) return false;
    return true;
}
int main()
{
    InitConsole();
    SqList L{};
    try {
        while (true) {
            cout << "\n顺序表：1初始化 2建表 3取值 4查找" << endl;
            cout << "5插入 6删除 7归并 8输出 9清空 0退出" << endl;
            int op = ReadNumber<int>("请选择：");
            if (op == 0) break;
            if (op == 1) {
                ClearList(L); InitList(L);
                cout << "初始化完成，表长=0" << endl;
                continue;
            }
            if (!L.elem) { cout << "请先初始化顺序表。" << endl; continue; }
            if (op == 2 || op == 7) {
                int n = ReadNumber<int>("教材数量：");
                if (n < 0 || n > MAXSIZE) { cout << "数量不合法。" << endl; continue; }
                vector<Book> books;
                for (int i = 0; i < n; ++i) {
                    cout << "第" << i + 1 << "本教材" << endl;
                    books.push_back(ReadBook());
                }
                if (op == 2) {
                    for (int i = 0; i < n; ++i) L.elem[i] = books[i];
                    L.length = n;
                    cout << "建表完成，表长=" << L.length << endl;
                } else {
                    SqList A{}, B{}; InitList(A); InitList(B);
                    for (int i = 0; i < n; ++i) A.elem[i] = books[i];
                    A.length = n;
                    if (!IsSorted(L) || !IsSorted(A))
                        cout << "归并失败：输入表未按ISBN非递减排列。" << endl;
                    else if (!MergeList(L, A, B))
                        cout << "归并失败：总长度超过容量。" << endl;
                    else {
                        cout << "归并结果：" << endl;
                        for (int i = 0; i < B.length; ++i) PrintBook(B.elem[i]);
                        cout << "归并后表长=" << B.length << endl;
                    }
                    ClearList(A); ClearList(B);
                }
            } else if (op == 3) {
                Book b; int i = ReadNumber<int>("取值位序：");
                if (GetElem(L, i, b)) PrintBook(b);
                else cout << "位序不合法。" << endl;
            } else if (op == 4) {
                string isbn; cout << "查找ISBN：";
                if (!getline(cin, isbn)) break;
                cout << "位序=" << LocateElem(L, isbn) << "（0表示未找到）" << endl;
            } else if (op == 5) {
                int i = ReadNumber<int>("插入位序：");
                if (i < 1 || i > L.length + 1 || L.length == MAXSIZE) {
                    cout << "位序不合法或顺序表已满。" << endl; continue;
                }
                Book b = ReadBook(); ListInsert(L, i, b);
                cout << "插入成功，表长=" << L.length << endl;
            } else if (op == 6) {
                int i = ReadNumber<int>("删除位序：");
                cout << (ListDelete(L, i) ? "删除成功。" : "位序不合法。") << endl;
            } else if (op == 8) {
                if (!L.length) cout << "顺序表为空。" << endl;
                for (int i = 0; i < L.length; ++i) PrintBook(L.elem[i]);
            } else if (op == 9) {
                ClearList(L); InitList(L);
                cout << "清空完成，表长=0" << endl;
            } else cout << "无效选项。" << endl;
        }
    } catch (const exception& e) { cout << e.what() << endl; }
    ClearList(L);
    return 0;
}
