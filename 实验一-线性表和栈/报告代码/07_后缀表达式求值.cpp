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

template<class T> class LinkStack {
    struct Node { T data; Node* next; };
    Node* top = nullptr;
    size_t count = 0;
public:
    LinkStack() = default;
    LinkStack(const LinkStack&) = delete;
    LinkStack& operator=(const LinkStack&) = delete;
    ~LinkStack() { clear(); }
    bool empty() const { return top == nullptr; }
    size_t size() const { return count; }
    void clear() {
        while (top) { Node* q = top; top = q->next; delete q; }
        count = 0;
    }
    bool push(const T& x) {
        top = new Node{x, top}; ++count; return true;
    }
    bool pop(T& x) {
        if (empty()) return false;
        Node* q = top; x = q->data; top = q->next;
        delete q; --count; return true;
    }
    bool peek(T& x) const {
        if (empty()) return false;
        x = top->data; return true;
    }
};
bool isOp(char c) { return c == '+' || c == '-' || c == '*' || c == '/'; }
template<class ValueStack> double evaluate(const string& text) {
    ValueStack values; double a, b, r;
    for (unsigned char byte : text) {
        if (isspace(byte)) continue;
        char c = static_cast<char>(byte);
        if (c >= '0' && c <= '9') {
            if (!values.push(c - '0')) throw runtime_error("操作数栈已满");
        } else if (isOp(c)) {
            // 第一次弹出的是右操作数，减法与除法不能交换顺序。
            if (!values.pop(b) || !values.pop(a)) throw runtime_error("后缀表达式缺少操作数");
            if (c == '+') r = a + b;
            else if (c == '-') r = a - b;
            else if (c == '*') r = a * b;
            else { if (b == 0.0) throw runtime_error("除数不能为零"); r = a / b; }
            if (!isfinite(r)) throw runtime_error("计算结果溢出");
            if (!values.push(r)) throw runtime_error("操作数栈已满");
        } else throw runtime_error("后缀表达式包含非法字符");
    }
    if (values.size() != 1 || !values.pop(r)) throw runtime_error("后缀表达式结构错误");
    return r;
}



int main()
{
    InitConsole();
    cout << "后缀表达式求值（输入exit退出）" << endl;
    string expression;
    while (true) {
        cout << "请输入表达式：";
        if (!getline(cin, expression) || expression == "exit") break;
        try {
            double result = evaluate<LinkStack<double>>(expression);
            cout << "结果：" << result << endl;
        } catch (const exception& e) {
            cout << "错误：" << e.what() << endl;
        }
    }
    return 0;
}
