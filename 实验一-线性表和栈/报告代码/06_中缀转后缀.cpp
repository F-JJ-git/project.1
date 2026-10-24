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
public:
    void clear() { top = 0; }
    bool empty() const { return top == 0; }
    size_t size() const { return top; }
    bool push(const T& x) {
        if (top == Capacity) return false;
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
bool leftBracket(char c) { return c == '(' || c == '[' || c == '{'; }
bool rightBracket(char c) { return c == ')' || c == ']' || c == '}'; }
bool pairBracket(char a, char b) {
    return (a == '(' && b == ')') || (a == '[' && b == ']') || (a == '{' && b == '}');
}
template<class Stack> bool brackets(const string& text) {
    Stack s; char c;
    for (char x : text) {
        if (leftBracket(x)) { if (!s.push(x)) return false; }
        else if (rightBracket(x)) {
            if (!s.pop(c) || !pairBracket(c, x)) return false;
        }
    }
    return s.empty();
}
int priority(char c) { return (c == '+' || c == '-') ? 1 : 2; }
bool isOp(char c) { return c == '+' || c == '-' || c == '*' || c == '/'; }

// 支持个位数非负操作数和二元四则运算；三种括号均可用于分组。
// expectOperand 检查语法，防止 12、2(3)、空括号及连续运算符。
template<class OpStack> string postfix(const string& infix) {
    OpStack ops; string result; bool expectOperand = true;
    auto emit = [&](char c) { if (!result.empty()) result += ' '; result += c; };
    auto push = [&](char c) { if (!ops.push(c)) throw runtime_error("运算符栈已满"); };
    char t;
    for (unsigned char byte : infix) {
        if (isspace(byte)) continue;
        char c = static_cast<char>(byte);
        if (c >= '0' && c <= '9') {
            if (!expectOperand) throw runtime_error("缺少运算符或输入了多位数");
            emit(c); expectOperand = false;
        } else if (leftBracket(c)) {
            if (!expectOperand) throw runtime_error("左括号前缺少运算符");
            push(c);
        } else if (rightBracket(c)) {
            if (expectOperand) throw runtime_error("空括号或右括号前缺少操作数");
            while (ops.peek(t) && !leftBracket(t)) { ops.pop(t); emit(t); }
            if (!ops.pop(t) || !pairBracket(t, c)) throw runtime_error("括号不匹配");
            expectOperand = false;
        } else if (isOp(c)) {
            if (expectOperand) throw runtime_error("运算符前缺少操作数");
            while (ops.peek(t) && isOp(t) && priority(t) >= priority(c)) {
                ops.pop(t); emit(t);
            }
            push(c); expectOperand = true;
        } else throw runtime_error("包含非法字符");
    }
    if (expectOperand) throw runtime_error("表达式为空或末尾缺少操作数");
    while (ops.pop(t)) {
        if (leftBracket(t)) throw runtime_error("左括号未闭合");
        emit(t);
    }
    return result;
}



int main()
{
    InitConsole();
    cout << "中缀表达式转后缀表达式（输入exit退出）" << endl;
    string expression;
    while (true) {
        cout << "请输入表达式：";
        if (!getline(cin, expression) || expression == "exit") break;
        try {
            string result = postfix<SeqStack<char>>(expression);
            cout << "后缀：" << result << endl;
        } catch (const exception& e) {
            cout << "错误：" << e.what() << endl;
        }
    }
    return 0;
}
