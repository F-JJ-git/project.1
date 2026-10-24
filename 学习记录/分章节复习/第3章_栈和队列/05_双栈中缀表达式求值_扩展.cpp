/*
双栈中缀表达式求值（扩展）
课件：第3章*3.3，第56—65、67—84张；第85—105张另给栈内/栈外优先数方法。
输入一行表达式；输出double结果或错误。支持多位非负十进制数、小数、二元 + - * / 和圆括号。
课件用LinkStack和'='哨兵，本版用std::stack体现同样的双栈归约思想，以串尾结束。
expectOperand补充完整语法检查；不支持一元正负号、科学计数法、隐式乘法、其他括号。
每个符号至多入/出栈一次，时间O(n)、辅助空间O(n)。带星号是课件标记，不代替考试范围通知。
*/

#include <iostream>
#include <stack>
#include <string>
#include <cctype>
#include <cmath>
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

int Precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

void ApplyTop(stack<double>& values, stack<char>& ops) {
    if (values.size() < 2 || ops.empty()) throw runtime_error("表达式缺少操作数");
    // 第一次弹出的是右操作数；减法和除法不能颠倒a、b。
    double b = values.top(); values.pop();
    double a = values.top(); values.pop();
    char op = ops.top(); ops.pop();
    double result = 0;
    if (op == '+') result = a + b;
    else if (op == '-') result = a - b;
    else if (op == '*') result = a * b;
    else if (op == '/') {
        if (b == 0) throw runtime_error("除数不能为0");
        result = a / b;
    } else throw runtime_error("无效运算符");
    if (!isfinite(result)) throw runtime_error("计算结果溢出");
    values.push(result);
}

double Evaluate(string text) {
    // 兼容课件的等号结束写法，也允许直接按回车结束；串内的等号仍非法。
    while (!text.empty() && isspace(static_cast<unsigned char>(text.back()))) text.pop_back();
    if (!text.empty() && text.back() == '=') text.pop_back();
    stack<double> values; // 保存尚未参与归约的数值和已经计算的子表达式值。
    stack<char> ops;      // 保存尚未执行的运算符和左括号。
    bool expectOperand = true;
    for (size_t i = 0; i < text.size();) {
        unsigned char byte = static_cast<unsigned char>(text[i]);
        if (isspace(byte)) { ++i; continue; }
        char c = text[i];
        if (isdigit(byte) || c == '.') {
            if (!expectOperand) throw runtime_error("两个操作数之间缺少运算符");
            size_t begin = i;
            bool digitSeen = false;
            while (i < text.size() && isdigit(static_cast<unsigned char>(text[i]))) { ++i; digitSeen = true; }
            if (i < text.size() && text[i] == '.') {
                ++i;
                while (i < text.size() && isdigit(static_cast<unsigned char>(text[i]))) { ++i; digitSeen = true; }
            }
            if (!digitSeen) throw runtime_error("小数格式错误");
            double value = stod(text.substr(begin, i - begin));
            if (!isfinite(value)) throw runtime_error("操作数必须为有限数");
            values.push(value); expectOperand = false;
        } else if (c == '(') {
            if (!expectOperand) throw runtime_error("左括号前缺少运算符");
            ops.push(c); ++i;
        } else if (c == ')') {
            if (expectOperand) throw runtime_error("空括号或右括号前缺少操作数");
            while (!ops.empty() && ops.top() != '(') ApplyTop(values, ops);
            if (ops.empty()) throw runtime_error("括号不匹配");
            ops.pop(); ++i; expectOperand = false;
        } else if (c == '+' || c == '-' || c == '*' || c == '/') {
            if (expectOperand) throw runtime_error("运算符前缺少操作数（不支持一元正负号）");
            // 同级也先归约，保证8-3-2按(8-3)-2计算，不能写成严格大于。
            while (!ops.empty() && ops.top() != '(' && Precedence(ops.top()) >= Precedence(c))
                ApplyTop(values, ops);
            ops.push(c); ++i; expectOperand = true;
        } else throw runtime_error("存在不支持的字符");
    }
    if (expectOperand) throw runtime_error("表达式为空或末尾缺少操作数");
    while (!ops.empty()) {
        if (ops.top() == '(') throw runtime_error("括号不匹配");
        ApplyTop(values, ops);
    }
    if (values.size() != 1) throw runtime_error("表达式格式错误");
    return values.top();
}

int main() {
    InitConsole();
    string expression;
    cout << "输入表达式（十进制非负整数/小数，+-*/和圆括号，可用=结束）：\n";
    if (!getline(cin, expression)) { cout << "未读取到表达式。\n"; return 1; }
    try {
        double result = Evaluate(expression);
        cout << "计算结果：" << result << '\n';
    } catch (const exception& ex) { cout << "错误：" << ex.what() << '\n'; }
}
