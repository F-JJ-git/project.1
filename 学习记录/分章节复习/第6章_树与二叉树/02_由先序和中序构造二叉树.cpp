/*
 * 课件第84～93页：由先序和中序序列建立二叉树。
 * 核心：先序首元素是根；根在中序中的位置划分左右子树，再递归处理区间。
 * 输入：先序、中序各一行，结点是互不重复的可见ASCII字符，空白忽略；两行#表示空树。
 *       先序输入空行或直接结束输入时，使用原示例ABDCE / DBAEC。
 * 输出：先序/中序验证，以及后序、层次序列。
 * 复杂度：哈希表预存中序位置后，平均O(n)时间、O(n)表空间及O(h)递归空间。
 * 课件逐次扫描中序位置的写法最坏O(n^2)；这里仅替换查位置方式，区间划分相同。
 */
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stdexcept>
#include <cctype>
#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif
using namespace std;

struct Node {
    char data;
    Node* left = nullptr;
    Node* right = nullptr;
    explicit Node(char c) : data(c) {}
};
void Destroy(Node* r) {
    if (!r) return;
    Destroy(r->left); Destroy(r->right); delete r;
}
Node* Build(const string& preorder, int preL, int preR, int inL, int inR,
            const unordered_map<char, int>& position) {
    if (preL > preR) {
        if (inL <= inR) throw runtime_error("子树区间长度不一致");
        return nullptr;
    }
    if (preR - preL != inR - inL) throw runtime_error("子树区间长度不一致");
    const char rootValue = preorder[preL];
    const auto found = position.find(rootValue);
    if (found == position.end() || found->second < inL || found->second > inR)
        throw runtime_error("遍历序列不匹配");
    const int middle = found->second;
    const int leftSize = middle - inL;
    Node* root = new Node(rootValue);
    try {
        // 左子树在先序中占leftSize个元素；右子树从这段之后开始。
        root->left = Build(preorder, preL + 1, preL + leftSize, inL, middle - 1, position);
        root->right = Build(preorder, preL + leftSize + 1, preR, middle + 1, inR, position);
    } catch (...) {
        Destroy(root); // 右子树失败时，已经建好的根和左子树也必须释放。
        throw;
    }
    return root;
}
void Pre(const Node* r) {
    if (!r) return;
    cout << r->data; Pre(r->left); Pre(r->right);
}
void In(const Node* r) {
    if (!r) return;
    In(r->left); cout << r->data; In(r->right);
}
void Post(const Node* r) {
    if (!r) return;
    Post(r->left); Post(r->right); cout << r->data;
}
void Level(const Node* r) {
    if (!r) return;
    queue<const Node*> waiting;
    waiting.push(r);
    while (!waiting.empty()) {
        const Node* p = waiting.front(); waiting.pop(); cout << p->data;
        if (p->left) waiting.push(p->left);
        if (p->right) waiting.push(p->right);
    }
}
string Compact(const string& input) {
    string result;
    for (unsigned char c : input) {
        if (isspace(c)) continue;
        if (c < 33 || c > 126) throw runtime_error("结点必须为单个可见ASCII字符");
        result += static_cast<char>(c);
    }
    return result;
}
int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    Node* root = nullptr;
    try {
        string pre, in;
        cout << "输入先序序列（空行用示例，#为空树）：";
        if (!getline(cin, pre) || pre.empty()) { pre = "ABDCE"; in = "DBAEC"; }
        else {
            cout << "输入中序序列：";
            if (!getline(cin, in)) throw runtime_error("缺少中序序列");
        }
        pre = Compact(pre); in = Compact(in);
        if (pre == "#") pre.clear();
        if (in == "#") in.clear();
        if (pre.size() != in.size()) throw runtime_error("序列长度不一致");
        if (pre.size() > 256) throw runtime_error("本练习最多256个结点");
        unordered_map<char, int> position;
        unordered_set<char> seen;
        for (int i = 0; i < static_cast<int>(in.size()); ++i)
            if (!position.emplace(in[i], i).second) throw runtime_error("结点值不能重复");
        for (char c : pre)
            if (!seen.insert(c).second) throw runtime_error("结点值不能重复");
        // 字符集合相同也可能排列矛盾，Build还会检查每一层的子树区间。
        root = Build(pre, 0, static_cast<int>(pre.size()) - 1,
                     0, static_cast<int>(in.size()) - 1, position);
        cout << "先序验证："; Pre(root); cout << '\n';
        cout << "中序验证："; In(root); cout << '\n';
        cout << "后序结果："; Post(root); cout << '\n';
        cout << "层次结果："; Level(root); cout << '\n';
        if (!root) cout << "已构造空树。\n";
        Destroy(root);
    } catch (const exception& e) {
        Destroy(root); cout << "构造失败：" << e.what() << '\n'; return 1;
    }
    return 0;
}
