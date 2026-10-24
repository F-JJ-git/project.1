/*
 * 课件第154～171页：路径长度、WPL、每轮合并最小两权、前缀编码。
 * 第175～177页的完整HuffmanTree类模板标注“不讲”；本文件是算法练习。
 * 输入：n（1至94），随后n行“单个可见ASCII字符 正整数权值”，权值不超过10^9。
 * 输出：每个叶子的编码、树上深度、根权值、WPL；相等权按较早建立的结点优先。
 * 复杂度：每轮线性扫描选择两个最小根，构造O(n^2)；存储O(n)，全部码字最坏O(n^2)。
 */
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <set>
#include <sstream>
#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif
using namespace std;

struct HuffmanNode {
    long long weight = 0;
    int parent = -1, left = -1, right = -1;
};
int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    int n = 0;
    cout << "输入字符个数 n（1至94）：";
    string countLine;
    if (!getline(cin, countLine)) { cout << "字符个数不合法。\n"; return 1; }
    istringstream countInput(countLine);
    char trailing;
    if (!(countInput >> n) || (countInput >> trailing) || n < 1 || n > 94) {
        cout << "字符个数不合法。\n"; return 1;
    }
    vector<char> chars(n);
    // n个叶子、n-1个新父结点；前n项存放输入叶子。
    vector<HuffmanNode> nodes(2 * n - 1);
    set<char> used;
    cout << "依次输入每个字符及其正整数权值，例如 A 5：\n";
    for (int i = 0; i < n; ++i) {
        string line;
        if (!getline(cin >> ws, line)) { cout << "字符或权值输入格式错误。\n"; return 1; }
        istringstream input(line);
        char extra;
        // 按整行检查，防止把2.5或2abc的前缀2误当成完整整数权值。
        if (!(input >> chars[i] >> nodes[i].weight) || (input >> extra)) {
            cout << "字符或权值输入格式错误。\n"; return 1;
        }
        const unsigned char byte = static_cast<unsigned char>(chars[i]);
        if (byte < 33 || byte > 126 || !used.insert(chars[i]).second) {
            cout << "字符须为互不相同的单个可见ASCII字符。\n"; return 1;
        }
        if (nodes[i].weight <= 0 || nodes[i].weight > 1000000000LL) {
            cout << "权值须为1至1000000000的整数。\n"; return 1;
        }
    }
    // parent==-1表示该结点还是森林中一棵树的根，才有资格参加下一轮选择。
    for (int i = n; i < 2 * n - 1; ++i) {
        int s1 = -1, s2 = -1;
        for (int j = 0; j < i; ++j) {
            if (nodes[j].parent != -1) continue;
            if (s1 == -1 || nodes[j].weight < nodes[s1].weight) {
                s2 = s1; s1 = j;
            } else if (s2 == -1 || nodes[j].weight < nodes[s2].weight) s2 = j;
        }
        nodes[s1].parent = i; nodes[s2].parent = i;
        nodes[i].left = s1; nodes[i].right = s2;
        nodes[i].weight = nodes[s1].weight + nodes[s2].weight;
        // 至多94个叶子且权值<=10^9，总和及本练习WPL均处于long long范围内。
    }
    long long wpl = 0;
    cout << "哈夫曼编码：\n";
    for (int i = 0; i < n; ++i) {
        string code;
        int depth = 0;
        for (int cur = i; nodes[cur].parent != -1; cur = nodes[cur].parent) {
            const int parent = nodes[cur].parent;
            code += (nodes[parent].left == cur ? '0' : '1');
            ++depth;
        }
        // 从叶子回溯得到的位序与根到叶子相反，必须反转。
        reverse(code.begin(), code.end());
        if (n == 1) code = "0"; // 单字符编码约定；树上叶深度仍为0。
        cout << chars[i] << " : " << code << "，编码长度=" << code.size()
             << "，树上深度=" << depth << '\n';
        wpl += nodes[i].weight * depth;
    }
    cout << "根结点权值：" << nodes.back().weight << '\n';
    cout << "WPL（叶子权值×树上深度之和）：" << wpl << '\n';
    if (n == 1) cout << "单字符约定编码为0；一位编码不改变树上深度0，因此WPL=0。\n";
    cout << "相同权值可能产生不同编码，但最优WPL相同。\n";
    return 0;
}
