// 第5章：广义表概念练习，课件幻灯片58～60（定义）、73～75（深度）。
// 65页将存储结构标注“不讲”；这里的解析树只为验证概念，不实现引用数法广义表。
// 输入：一行有限、无共享/自引用的广义表，如(a,(b,c),())；原子限单个ASCII字母或数字。
// 空表()长0、深度1，无表头和表尾；单元素表的表尾为()。原子的深度为0。
// 输入最多4096字符、括号最多128层。解析、深度和显示均O(n)，存储O(n)，递归栈O(h)。
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <cctype>
#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif
using namespace std;

struct Element {
    bool atom = false;
    char value = 0;
    vector<Element> children; // 对表而言，children只保存直接元素，而非全部后代。
};

class Parser {
    const string& text;
    size_t pos = 0;
    void SkipSpaces() {
        while (pos < text.size() && isspace(static_cast<unsigned char>(text[pos]))) ++pos;
    }
    Element ParseElement(int depth) {
        SkipSpaces();
        if (pos == text.size()) throw runtime_error("缺少元素或右括号");
        Element result;
        if (text[pos] != '(') {
            const char c = text[pos];
            if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')))
                throw runtime_error("原子须为单个ASCII字母或数字");
            result.atom = true; result.value = c; ++pos;
            return result;
        }
        if (depth > 128) throw runtime_error("括号嵌套不能超过128层");
        ++pos; SkipSpaces();
        if (pos < text.size() && text[pos] == ')') { ++pos; return result; }
        while (true) {
            result.children.push_back(ParseElement(depth + 1));
            SkipSpaces();
            if (pos == text.size()) throw runtime_error("左括号未闭合");
            const char separator = text[pos++];
            if (separator == ')') break;
            if (separator != ',') throw runtime_error("元素之间须有逗号，且原子只能是单个字符");
            // 下一轮若遇右括号/逗号，就会拒绝(a,)和(a,,b)，不会吞掉空元素。
        }
        return result;
    }
public:
    explicit Parser(const string& input) : text(input) {}
    Element ParseList() {
        if (text.size() > 4096) throw runtime_error("输入不能超过4096字符");
        Element result = ParseElement(1);
        SkipSpaces();
        if (pos != text.size()) throw runtime_error("广义表末尾有多余字符");
        if (result.atom) throw runtime_error("请输入带外层括号的广义表");
        return result;
    }
};

int Depth(const Element& e)
{
    if (e.atom) return 0;
    int maxSubDepth = 0;
    for (const Element& child : e.children) maxSubDepth = max(maxSubDepth, Depth(child));
    return maxSubDepth + 1; // 空表也返回1；只含原子的表深度仍为1。
}

void AppendElement(string& output, const Element& e)
{
    if (e.atom) { output += e.value; return; }
    output += '(';
    for (size_t i = 0; i < e.children.size(); ++i) {
        if (i != 0) output += ',';
        AppendElement(output, e.children[i]);
    }
    output += ')';
}

int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    string text;
    cout << "输入广义表，例如(a,(b,c),())：\n";
    if (!getline(cin, text)) { cout << "错误：没有输入。\n"; return 1; }
    try {
        const Element list = Parser(text).ParseList();
        cout << "长度：" << list.children.size() << '\n';
        cout << "深度：" << Depth(list) << '\n';
        if (list.children.empty()) {
            cout << "表头：不存在（空表没有表头）\n";
            cout << "表尾：不存在（空表没有表尾）\n";
        } else {
            string head, tail = "(";
            AppendElement(head, list.children[0]);
            for (size_t i = 1; i < list.children.size(); ++i) {
                if (i != 1) tail += ',';
                AppendElement(tail, list.children[i]);
            }
            tail += ')'; // 表尾始终是一个表，而表头既可能是原子，也可能是子表。
            cout << "表头：" << head << '\n';
            cout << "表尾：" << tail << '\n';
        }
    } catch (const exception& e) { cout << "错误：" << e.what() << '\n'; return 1; }
    return 0;
}
