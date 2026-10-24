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



int main()
{
    InitConsole();
    cout << "括号匹配检查（输入exit退出）" << endl;
    string expression;
    while (true) {
        cout << "请输入表达式：";
        if (!getline(cin, expression) || expression == "exit") break;
        try {
            cout << "结果：" << (brackets<SeqStack<char>>(expression) ? "括号匹配" : "括号不匹配") << endl;
        } catch (const exception& e) {
            cout << "错误：" << e.what() << endl;
        }
    }
    return 0;
}
