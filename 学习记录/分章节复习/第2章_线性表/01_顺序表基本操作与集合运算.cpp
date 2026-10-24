/*
顺序表基本操作与集合运算
课件：第2章第7—12、14—30、73张幻灯片。
目标：把“位序”和“下标”分清，掌握插入后移、删除前移以及集合并/交/差。
输入：1运行固定课件数据；2检查空表、满表和非法位序。输出：各操作结果。
复杂度：取值/赋值O(1)，查找/插入/删除O(n)；交/差O(nm)，并集最坏O(m(n+m))。
这是int版独立练习，省略课件完整模板与深拷贝接口；类禁止复制，避免裸指针浅拷贝。
*/

#include <iostream>
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

class SeqList {
private:
    int* data;
    int count;
    int maxSize;
public:
    explicit SeqList(int capacity = 100) : data(nullptr), count(0), maxSize(capacity) {
        if (capacity < 0) throw invalid_argument("容量不能为负数");
        data = new int[capacity]; // 容量为0时，所有插入都被判满条件拦截。
    }
    ~SeqList() { delete[] data; }
    SeqList(const SeqList&) = delete;
    SeqList& operator=(const SeqList&) = delete;

    int Capacity() const { return maxSize; }
    int Length() const { return count; }
    bool Empty() const { return count == 0; }
    void Clear() { count = 0; }

    bool GetElem(int position, int& e) const {
        if (position < 1 || position > count) return false;
        // 逻辑位序从1开始，数组下标从0开始。
        e = data[position - 1];
        return true;
    }
    bool SetElem(int position, int e) {
        if (position < 1 || position > count) return false;
        data[position - 1] = e;
        return true;
    }
    int LocateElem(int e) const {
        for (int i = 0; i < count; ++i)
            if (data[i] == e) return i + 1; // 返回从 1 开始的位序
        return 0; // 0 表示未找到
    }
    bool Insert(int position, int e) {
        if (count == maxSize || position < 1 || position > count + 1) return false;
        // 必须从后向前搬移，否则会覆盖尚未搬走的元素。
        for (int i = count; i >= position; --i) data[i] = data[i - 1];
        data[position - 1] = e;
        ++count;
        return true;
    }
    bool Delete(int position, int& e) {
        if (position < 1 || position > count) return false;
        e = data[position - 1];
        // 删除则相反：从前向后填补空位。
        for (int i = position - 1; i < count - 1; ++i) data[i] = data[i + 1];
        --count;
        return true;
    }
    bool Contains(int e) const { return LocateElem(e) != 0; }
    void Display() const {
        cout << "(";
        for (int i = 0; i < count; ++i) {
            if (i) cout << ", ";
            cout << data[i];
        }
        cout << ")\n";
    }
};

// 集合运算约定输入表自身不含重复元素；结果保持A原有的先后次序。
bool UnionInto(SeqList& a, const SeqList& b) {
    int e = 0, missing = 0;
    for (int i = 1; i <= b.Length(); ++i) {
        b.GetElem(i, e);
        // LocateElem(e)==i 只计算B中第一次出现的值，避免重复计数。
        if (!a.Contains(e) && b.LocateElem(e) == i) ++missing;
    }
    if (missing > a.Capacity() - a.Length()) return false; // 先检查，失败时不改A。
    for (int i = 1; i <= b.Length(); ++i) {
        b.GetElem(i, e);
        if (!a.Contains(e)) a.Insert(a.Length() + 1, e);
    }
    return true;
}

bool Difference(const SeqList& a, const SeqList& b, SeqList& result) {
    if (&result == &a || &result == &b) return false; // 输出不能与输入共用对象。
    int e = 0, needed = 0;
    for (int i = 1; i <= a.Length(); ++i) {
        a.GetElem(i, e);
        if (!b.Contains(e)) ++needed;
    }
    if (needed > result.Capacity()) return false;
    result.Clear();
    for (int i = 1; i <= a.Length(); ++i) {
        a.GetElem(i, e);
        if (!b.Contains(e)) result.Insert(result.Length() + 1, e);
    }
    return true;
}

// 对应课件第73张的交集作业：只保留同时出现在A、B中的元素。
bool Intersection(const SeqList& a, const SeqList& b, SeqList& result) {
    if (&result == &a || &result == &b) return false;
    int e = 0, needed = 0;
    for (int i = 1; i <= a.Length(); ++i) {
        a.GetElem(i, e);
        if (b.Contains(e)) ++needed;
    }
    if (needed > result.Capacity()) return false;
    result.Clear();
    for (int i = 1; i <= a.Length(); ++i) {
        a.GetElem(i, e);
        if (b.Contains(e)) result.Insert(result.Length() + 1, e);
    }
    return true;
}

int main() {
    InitConsole();
    int mode = 1;
    cout << "1.课件示例  2.边界检查（默认1）：";
    if (!(cin >> mode)) mode = 1;
    if (mode == 2) {
        SeqList a(1), b(1), out(1);
        int e = 99;
        cout << "空表取值：" << a.GetElem(1, e) << "，输出参数=" << e << '\n';
        cout << "非法位序插入：" << a.Insert(0, 1) << '\n';
        a.Insert(1, 1); b.Insert(1, 2);
        cout << "满表插入：" << a.Insert(2, 3) << '\n';
        cout << "容量不足的并集：" << UnionInto(a, b) << "，A保持："; a.Display();
        cout << "非法位序删除：" << a.Delete(2, e) << '\n';
        return 0;
    }
    SeqList a(30), b(30), result(30);
    for (int x : {1, 3, 7, 9, 12}) a.Insert(a.Length() + 1, x);
    for (int x : {1, 2, 8, 9, 11}) b.Insert(b.Length() + 1, x);
    cout << "初始 A："; a.Display();
    cout << "初始 B："; b.Display();
    Difference(a, b, result); cout << "原A-B："; result.Display();
    Intersection(a, b, result); cout << "交集A∩B："; result.Display();
    if (UnionInto(a, b)) { cout << "并集A∪B："; a.Display(); }
    a.Insert(2, 100); cout << "位序2插入100："; a.Display();
    int removed = 0, e = 0;
    if (a.Delete(2, removed)) cout << "删除位序2：" << removed << '\n';
    a.SetElem(1, 6); a.GetElem(1, e);
    cout << "第1个元素改为：" << e << '\n';
    cout << "查找9的位序：" << a.LocateElem(9) << '\n';
}
