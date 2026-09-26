#include <iostream>
#include <string>

using namespace std;

#define MAXSIZE 1000

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

// 2. 建表
void CreateList(SqList& L, int n)
{
    if (n > MAXSIZE || n < 0)
    {
        cout << "教材数量不合法！" << endl;
        return;
    }

    for (int i = 0; i < n; i++)
    {
        cout << "请输入第 " << i + 1 << " 本书的ISBN：";
        cin >> L.elem[i].ISBN;

        cout << "请输入第 " << i + 1 << " 本书的出版社：";
        cin >> L.elem[i].publisher;

        cout << "请输入第 " << i + 1 << " 本书的作者：";
        cin >> L.elem[i].author;

        cout << "请输入第 " << i + 1 << " 本书的价格：";
        cin >> L.elem[i].price;
    }

    L.length = n;
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

// 8. 输出
void DisplayList(SqList L)
{
    if (L.length == 0)
    {
        cout << "当前顺序表为空！" << endl;
        return;
    }

    for (int i = 0; i < L.length; i++)
    {
        cout << "第 " << i + 1 << " 本书的ISBN：" << L.elem[i].ISBN << endl;
        cout << "第 " << i + 1 << " 本书的出版社：" << L.elem[i].publisher << endl;
        cout << "第 " << i + 1 << " 本书的作者：" << L.elem[i].author << endl;
        cout << "第 " << i + 1 << " 本书的价格：" << L.elem[i].price << endl;
        cout << endl;
    }
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

// 10. 菜单
void ShowMenu()
{
    cout << "******************************" << endl;
    cout << "***** 1. 初始化顺序表   *****" << endl;
    cout << "***** 2. 建立顺序表     *****" << endl;
    cout << "***** 3. 取值           *****" << endl;
    cout << "***** 4. 查找           *****" << endl;
    cout << "***** 5. 插入           *****" << endl;
    cout << "***** 6. 删除           *****" << endl;
    cout << "***** 7. 归并           *****" << endl;
    cout << "***** 8. 输出顺序表     *****" << endl;
    cout << "***** 9. 清空顺序表     *****" << endl;
    cout << "***** 0. 退出程序       *****" << endl;
    cout << "******************************" << endl;
}

int main()
{
    SqList L;
    L.elem = nullptr;
    L.length = 0;

    int select;

    while (1)
    {
        ShowMenu();

        cout << "请输入操作：";
        cin >> select;

        switch (select)
        {
        // 1. 初始化
        case 1:
        {
            if (L.elem != nullptr)
            {
                ClearList(L);
            }

            InitList(L);
            cout << "初始化顺序表成功！" << endl;
            break;
        }

        // 2. 建表
        case 2:
        {
            if (L.elem == nullptr)
            {
                cout << "请先初始化顺序表！" << endl;
                break;
            }

            int n;
            cout << "请输入教材数量：";
            cin >> n;

            if (n < 0 || n > MAXSIZE)
            {
                cout << "教材数量不合法！" << endl;
                break;
            }

            CreateList(L, n);
            cout << "建表完成！" << endl;
            break;
        }

        // 3. 取值
        case 3:
        {
            if (L.elem == nullptr)
            {
                cout << "请先初始化顺序表！" << endl;
                break;
            }

            Book e;
            int position;

            cout << "请输入要取第几本书：";
            cin >> position;

            if (GetElem(L, position, e))
            {
                cout << "该教材的信息如下：" << endl;
                cout << "ISBN：" << e.ISBN << endl;
                cout << "出版社：" << e.publisher << endl;
                cout << "作者：" << e.author << endl;
                cout << "价格：" << e.price << endl;
            }
            else
            {
                cout << "位序不合法！" << endl;
            }

            break;
        }

        // 4. 查找
        case 4:
        {
            if (L.elem == nullptr)
            {
                cout << "请先初始化顺序表！" << endl;
                break;
            }

            string ISBN;
            cout << "请输入查找教材的ISBN：";
            cin >> ISBN;

            int position = LocateElem(L, ISBN);

            if (position == 0)
            {
                cout << "未查找到该教材！" << endl;
            }
            else
            {
                cout << "该教材的位序为：" << position << endl;
            }

            break;
        }

        // 5. 插入
        case 5:
        {
            if (L.elem == nullptr)
            {
                cout << "请先初始化顺序表！" << endl;
                break;
            }

            Book e;
            int position;

            cout << "请输入插入位置：";
            cin >> position;

            cout << "请输入ISBN：";
            cin >> e.ISBN;

            cout << "请输入出版社：";
            cin >> e.publisher;

            cout << "请输入作者：";
            cin >> e.author;

            cout << "请输入价格：";
            cin >> e.price;

            if (ListInsert(L, position, e))
            {
                cout << "插入成功！" << endl;
            }
            else
            {
                cout << "插入位序不合法！" << endl;
            }

            break;
        }

        // 6. 删除
        case 6:
        {
            if (L.elem == nullptr)
            {
                cout << "请先初始化顺序表！" << endl;
                break;
            }

            int position;
            cout << "请输入删除元素的位序：";
            cin >> position;

            if (ListDelete(L, position))
            {
                cout << "删除成功！" << endl;
            }
            else
            {
                cout << "输入位序不合法！" << endl;
            }

            break;
        }

        // 7. 归并
        case 7:
        {
            if (L.elem == nullptr)
            {
                cout << "请先初始化顺序表！" << endl;
                break;
            }

            SqList A, B;
            InitList(A);
            InitList(B);

            int n;

            cout << "注意：L和A都应按ISBN非递减排列！" << endl;
            cout << "请输入顺序表A的教材数量：";
            cin >> n;

            if (n < 0 || n > MAXSIZE)
            {
                cout << "教材数量不合法！" << endl;
                ClearList(A);
                ClearList(B);
                break;
            }

            CreateList(A, n);

            if (MergeList(L, A, B))
            {
                cout << "归并成功！归并结果如下：" << endl;
                DisplayList(B);
            }
            else
            {
                cout << "归并失败！" << endl;
            }

            ClearList(A);
            ClearList(B);
            break;
        }

        // 8. 输出
        case 8:
        {
            if (L.elem == nullptr)
            {
                cout << "请先初始化顺序表！" << endl;
                break;
            }

            cout << "当前顺序表内容为：" << endl;
            DisplayList(L);
            break;
        }

        // 9. 清空
        case 9:
        {
            if (L.elem == nullptr)
            {
                cout << "顺序表尚未初始化！" << endl;
                break;
            }

            ClearList(L);
            InitList(L);

            cout << "顺序表已清空！" << endl;
            break;
        }

        // 0. 退出程序
        case 0:
        {
            if (L.elem != nullptr)
            {
                ClearList(L);
            }

            cout << "程序已退出！" << endl;
            return 0;
        }

        default:
        {
            cout << "输入错误，请重新输入！" << endl;
            break;
        }
        }

        cout << endl;
    }

    return 0;
}
