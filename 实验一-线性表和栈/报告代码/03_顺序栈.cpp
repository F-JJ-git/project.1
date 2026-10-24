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

template<class T, size_t Capacity = MAXSIZE> class SeqStack {
    array<T, Capacity> data{};
    size_t top = 0;
    size_t limit;
public:
    explicit SeqStack(size_t capacity = Capacity) : limit(capacity)
    {
        if (capacity == 0 || capacity > Capacity) throw runtime_error("容量不合法");
    }
    void clear() { top = 0; }
    bool empty() const { return top == 0; }
    size_t size() const { return top; }
    bool push(const T& x) {
        if (top == limit) return false;
        data[top++] = x; return true;
    }
    bool pop(T& x) {
        if (empty()) return false;
        x = data[--top]; return true;
    }
    bool peek(T& x) const {
        if (empty()) return false;
        x = data[top - 1]; return true;
    }
};



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

int main()
{
    InitConsole();
    try {
        int capacity;
        do { capacity = ReadNumber<int>("请输入栈容量（1至1000）："); }
        while (capacity < 1 || capacity > MAXSIZE);
        SeqStack<int> s(static_cast<size_t>(capacity));
        while (true) {
            cout << "\n顺序栈：1初始化/清空 2入栈 3出栈 4取顶" << endl;
            cout << "5判空 6元素数量 0退出" << endl;
            int op = ReadNumber<int>("请选择："), value;
            if (op == 0) break;
            if (op == 1) {
                s.clear(); cout << "初始化完成，栈为空。" << endl;
            } else if (op == 2) {
                value = ReadNumber<int>("入栈整数：");
                cout << (s.push(value) ? "入栈成功。" : "入栈失败，栈已满。") << endl;
            } else if (op == 3) {
                if (s.pop(value)) cout << "出栈元素=" << value << endl;
                else cout << "出栈失败，栈为空。" << endl;
            } else if (op == 4) {
                if (s.peek(value)) cout << "栈顶元素=" << value << endl;
                else cout << "取顶失败，栈为空。" << endl;
            } else if (op == 5) cout << (s.empty() ? "空栈" : "非空栈") << endl;
            else if (op == 6) cout << "元素数量=" << s.size() << endl;
            else cout << "无效选项。" << endl;
        }
    } catch (const exception& e) { cout << e.what() << endl; }
    return 0;
}
