/*
链栈的两个应用：括号匹配与回文判断
课件：第3章第18—28张（链栈和括号匹配）、第106张作业3.1（只将一半字符入栈）。
输入：选1后输入一行字符串判断回文；选2检查()[]{}，忽略其他字符。输出判断结果。
回文按字节比较，大小写、空格都算字符，适合ASCII练习；奇数长度跳过唯一中间字符。
空串是回文且括号匹配；两个算法时间O(n)，辅助栈空间最坏O(n)，无固定栈容量。
*/

#include <iostream>
#include <string>
#include <limits>

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

struct CharNode {
    char data;
    CharNode* next;
    explicit CharNode(char c, CharNode* n = nullptr) : data(c), next(n) {}
};

class CharStack {
private:
    CharNode* top;
public:
    CharStack() : top(nullptr) {}
    ~CharStack() { char c; while (Pop(c)) {} }
    CharStack(const CharStack&) = delete;
    CharStack& operator=(const CharStack&) = delete;
    bool Empty() const { return top == nullptr; }
    // 表头就是栈顶；新结点先链接旧栈顶，再更新top。
    void Push(char c) { top = new CharNode(c, top); }
    bool Pop(char& c) {
        if (!top) return false;
        // 先保存值和后继，再删除旧栈顶，避免访问已释放的结点。
        CharNode* p = top; c = p->data; top = p->next; delete p; return true;
    }
};

bool IsPalindrome(const string& s) {
    CharStack st;
    size_t n = s.size();
    // 入栈前半段，栈顶正好对应后半段接下来要比较的字符。
    for (size_t i = 0; i < n / 2; ++i) st.Push(s[i]);
    size_t i = (n % 2 == 0) ? n / 2 : n / 2 + 1; // 奇数长度跳过中间字符
    char c;
    for (; i < n; ++i) {
        if (!st.Pop(c) || c != s[i]) return false;
    }
    return st.Empty();
}

bool MatchBrackets(const string& s) {
    CharStack st;
    for (char c : s) {
        if (c == '(' || c == '[' || c == '{') st.Push(c);
        else if (c == ')' || c == ']' || c == '}') {
            char left;
            // 右括号只能匹配最近的未闭合左括号；不是任找一个同类括号。
            if (!st.Pop(left)) return false;
            if (!((left == '(' && c == ')') || (left == '[' && c == ']') || (left == '{' && c == '}'))) return false;
        }
    }
    return st.Empty();
}

int main() {
    InitConsole();
    int choice = 0;
    cout << "1.回文判断  2.括号匹配\n请选择：";
    if (!(cin >> choice)) { cout << "输入错误。\n"; return 1; }
    if (choice != 1 && choice != 2) { cout << "选项无效。\n"; return 1; }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string s;
    cout << "输入一行字符串（按ASCII字符比较，可含空格）：";
    if (!getline(cin, s)) { cout << "未读取到字符串。\n"; return 1; }
    if (choice == 1) cout << (IsPalindrome(s) ? "是回文\n" : "不是回文\n");
    else cout << (MatchBrackets(s) ? "括号匹配正确\n" : "括号匹配错误\n");
}
