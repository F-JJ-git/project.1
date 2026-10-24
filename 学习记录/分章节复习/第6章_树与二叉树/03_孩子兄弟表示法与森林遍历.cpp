/*
 * 课件第131～135、143、147～151页：孩子兄弟表示法及森林遍历。
 * 输入：1运行原示例森林，2检查空森林；无输入时运行示例。
 * 输出：森林先根/后根序列，以及首孩子、右兄弟的对应关系。
 * 关键：firstChild相当于二叉树左指针，rightSibling相当于二叉树右指针；
 *       森林先根=对应二叉树先序，森林后根=对应二叉树中序。
 * 复杂度：遍历、销毁均O(n)，递归空间O(hb)，hb为转换后二叉树的高度。
 */
#include <iostream>
#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif
using namespace std;

struct CSNode {
    char data;
    CSNode* firstChild = nullptr;
    CSNode* rightSibling = nullptr;
    explicit CSNode(char c) : data(c) {}
};
void BinaryPreOrder(const CSNode* r) {
    if (!r) return;
    cout << r->data << ' ';           // 第一棵树的根。
    BinaryPreOrder(r->firstChild);    // 第一棵树的子树森林。
    BinaryPreOrder(r->rightSibling);  // 森林中余下的树。
}
void BinaryInOrder(const CSNode* r) {
    if (!r) return;
    BinaryInOrder(r->firstChild);     // 先完成第一棵树的所有孩子。
    cout << r->data << ' ';           // 再访问该树的根。
    BinaryInOrder(r->rightSibling);   // 最后处理后面的树，符合森林后根定义。
}
void DestroyForest(CSNode*& r) {
    if (!r) return;
    DestroyForest(r->firstChild); DestroyForest(r->rightSibling); delete r;
    r = nullptr;
}
int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    cout << "1 示例森林  2 空森林：";
    int mode = 1;
    if (!(cin >> mode) && !cin.eof()) { cout << "输入必须是1或2。\n"; return 1; }
    if (mode != 1 && mode != 2) { cout << "输入必须是1或2。\n"; return 1; }
    CSNode* forest = nullptr;
    try {
        if (mode == 1) {
            // 森林有两棵树：P的孩子为Q、R；S是另一棵只有根的树。
            forest = new CSNode('P');
            forest->firstChild = new CSNode('Q');
            forest->firstChild->rightSibling = new CSNode('R');
            forest->rightSibling = new CSNode('S');
            // P到S的右指针连接两棵树的根，不表示S是P的孩子。
        }
        cout << "森林先根遍历（对应二叉树先序）：";
        BinaryPreOrder(forest); cout << '\n';
        cout << "森林后根遍历（对应二叉树中序）：";
        BinaryInOrder(forest); cout << '\n';
        if (forest) {
            cout << "P 的第一个孩子：" << forest->firstChild->data << '\n';
            cout << "Q 的右兄弟：" << forest->firstChild->rightSibling->data << '\n';
            cout << "第一棵树根 P 的右兄弟（第二棵树根）：" << forest->rightSibling->data << '\n';
        } else cout << "森林为空，遍历不访问任何结点。\n";
        DestroyForest(forest);
    } catch (const exception& e) {
        DestroyForest(forest); cout << "操作失败：" << e.what() << '\n'; return 1;
    }
    return 0;
}
