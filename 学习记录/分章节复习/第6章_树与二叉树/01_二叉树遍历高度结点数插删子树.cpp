/*
 * 第6章：二叉链表、遍历与子树操作。
 * 课件第16～17、47、50～51页；遍历第56～72页；计数/树高第78～80页；作业第178页。
 * 输入：1运行五结点示例，2检查空树；无输入时运行示例。
 * 输出：四种遍历、结点数、树高、数值和、最近共同祖先及插删子树后的结果。
 * 复杂度：遍历/计数/树高/求和/最近共同祖先均O(n)，递归辅助空间O(h)；
 *         层次遍历队列最多O(w)，其中h为树高、w为最大层宽。
 */
#include <iostream>
#include <queue>
#include <algorithm>
#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif
using namespace std;

struct Node {
    int data;
    Node* left = nullptr;
    Node* right = nullptr;
    explicit Node(int x) : data(x) {}
};

// 三种递归遍历的区别只是访问根的位置；空子树是共同的递归出口。
void PreOrder(const Node* r) {
    if (!r) return;
    cout << r->data << ' '; PreOrder(r->left); PreOrder(r->right);
}
void InOrder(const Node* r) {
    if (!r) return;
    InOrder(r->left); cout << r->data << ' '; InOrder(r->right);
}
void PostOrder(const Node* r) {
    if (!r) return;
    PostOrder(r->left); PostOrder(r->right); cout << r->data << ' ';
}
void LevelOrder(const Node* r) {
    if (!r) return;
    queue<const Node*> waiting;
    waiting.push(r);
    while (!waiting.empty()) {
        const Node* p = waiting.front(); waiting.pop();
        cout << p->data << ' ';
        // 队列先进先出；先放左孩子再放右孩子，保证同层从左向右。
        if (p->left) waiting.push(p->left);
        if (p->right) waiting.push(p->right);
    }
}
int NodeCount(const Node* r) {
    return r ? 1 + NodeCount(r->left) + NodeCount(r->right) : 0;
}
int Height(const Node* r) {
    // 课件按层数定义高度：空树高0，只有根结点的树高1。
    return r ? 1 + max(Height(r->left), Height(r->right)) : 0;
}
long long Sum(const Node* r) {
    return r ? static_cast<long long>(r->data) + Sum(r->left) + Sum(r->right) : 0;
}
void Destroy(Node*& r) {
    if (!r) return;
    // 后序释放：先使用孩子指针释放子树，再删除根。
    Destroy(r->left); Destroy(r->right); delete r;
    r = nullptr; // 传引用能同步清空调用者的子树入口。
}
void InsertLeftChild(Node* cur, int value) {
    if (!cur) return;
    Node* child = new Node(value);
    child->left = cur->left; // 原左子树挂到新结点左侧，不是丢弃。
    cur->left = child;
}
void InsertRightChild(Node* cur, int value) {
    if (!cur) return;
    Node* child = new Node(value);
    child->right = cur->right; // 原右子树挂到新结点右侧。
    cur->right = child;
}
void DeleteLeftChild(Node* cur) { if (cur) Destroy(cur->left); }
void DeleteRightChild(Node* cur) { if (cur) Destroy(cur->right); }

// 前提：p、q均非空且属于这棵树；按指针身份判断，数值相同不代表同一结点。
const Node* LowestCommonAncestor(const Node* root, const Node* p, const Node* q) {
    if (!root || root == p || root == q) return root;
    const Node* left = LowestCommonAncestor(root->left, p, q);
    const Node* right = LowestCommonAncestor(root->right, p, q);
    if (left && right) return root; // 两侧分别找到目标，当前根是首次汇合处。
    return left ? left : right;
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    cout << "1 五结点示例  2 空树检查：";
    int mode = 1;
    if (!(cin >> mode) && !cin.eof()) { cout << "输入必须是1或2。\n"; return 1; }
    if (mode != 1 && mode != 2) { cout << "输入必须是1或2。\n"; return 1; }
    Node* root = nullptr;
    try {
        if (mode == 1) {
            // 6的左/右孩子是3/9，3的左孩子是2，9的左孩子是7。
            root = new Node(6);
            root->left = new Node(3); root->right = new Node(9);
            root->left->left = new Node(2); root->right->left = new Node(7);
        }
        cout << "先序："; PreOrder(root); cout << '\n';
        cout << "中序："; InOrder(root); cout << '\n';
        cout << "后序："; PostOrder(root); cout << '\n';
        cout << "层次："; LevelOrder(root); cout << '\n';
        cout << "结点个数：" << NodeCount(root) << '\n';
        cout << "树高：" << Height(root) << '\n';
        cout << "结点值之和：" << Sum(root) << '\n';
        if (root) {
            const Node* lca = LowestCommonAncestor(root, root->left->left, root->right->left);
            cout << "结点2和7的最近共同祖先：" << lca->data << '\n';
            InsertLeftChild(root, 10);
            cout << "在根的左侧插入10后先序："; PreOrder(root); cout << '\n';
            DeleteLeftChild(root); // 删除10和原来的整棵左子树，不只删除结点10。
            cout << "删除根的左子树后先序："; PreOrder(root); cout << '\n';
            InsertRightChild(root, 11);
            cout << "在根的右侧插入11后先序："; PreOrder(root); cout << '\n';
            DeleteRightChild(root);
            cout << "删除根的右子树后先序："; PreOrder(root); cout << '\n';
        }
        Destroy(root);
        cout << "销毁后结点个数：" << NodeCount(root) << '\n';
    } catch (const exception& e) {
        Destroy(root); cout << "操作失败：" << e.what() << '\n'; return 1;
    }
    return 0;
}
