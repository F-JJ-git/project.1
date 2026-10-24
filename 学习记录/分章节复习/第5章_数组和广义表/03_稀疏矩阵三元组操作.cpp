// 第5章：三元组顺序表、设置/取值、简单转置、相加和相乘。
// 课件：幻灯片32～47，尤其43～45页按原列扫描的简单转置。
// 不变量：只存非零项；按(行,列)升序排列；一个位置最多一个三元组。
// 输入：先看固定示例，随后可输入A中的行列号查询；EOF直接结束。
// t为非零项数：Get/Set为O(t)，简单转置O(cols*t)，归并加法O(tA+tB)。
// 乘法保留课件的三重循环公式，查找也有成本，并非优化的稀疏乘法。
#include <iostream>
#include <vector>
#include <stdexcept>
#include <limits>
#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif
using namespace std;

struct Triple { int row, col, value; };

int CheckedInt(long long value)
{
    if (value < numeric_limits<int>::min() || value > numeric_limits<int>::max())
        throw overflow_error("整数运算结果超出int范围");
    return static_cast<int>(value);
}

class SparseMatrix {
    int rows, cols;
    vector<Triple> terms;
    static bool Before(const Triple& a, const Triple& b) {
        return a.row < b.row || (a.row == b.row && a.col < b.col);
    }
public:
    SparseMatrix(int r, int c) : rows(r), cols(c) {
        if (r < 1 || c < 1) throw invalid_argument("矩阵行列数必须为正数");
    }
    int Rows() const { return rows; }
    int Cols() const { return cols; }
    int Num() const { return static_cast<int>(terms.size()); }
    int GetElem(int r, int c) const {
        if (r < 1 || r > rows || c < 1 || c > cols) throw out_of_range("下标越界");
        for (const Triple& t : terms)
            if (t.row == r && t.col == c) return t.value;
        return 0; // 合法位置未记录才表示零；越界与零元素不是一回事。
    }
    bool SetElem(int r, int c, int value) {
        if (r < 1 || r > rows || c < 1 || c > cols) return false;
        const Triple key{r, c, value};
        auto it = terms.begin();
        while (it != terms.end() && Before(*it, key)) ++it;
        if (it != terms.end() && it->row == r && it->col == c) {
            if (value == 0) terms.erase(it); // 非零元改成0时，要删除记录。
            else it->value = value;
        } else if (value != 0) terms.insert(it, key);
        return true;
    }
    SparseMatrix Transpose() const {
        SparseMatrix result(cols, rows);
        // 原第col列变为新第col行。原表按行有序，所以取出的新列号也有序。
        // 仅交换行列号而不重排，会破坏三元组的行优先顺序。
        for (int col = 1; col <= cols; ++col)
            for (const Triple& t : terms)
                if (t.col == col) result.terms.push_back({t.col, t.row, t.value});
        return result;
    }
    static SparseMatrix Add(const SparseMatrix& a, const SparseMatrix& b) {
        if (a.rows != b.rows || a.cols != b.cols)
            throw invalid_argument("矩阵维度不一致，不能相加");
        SparseMatrix result(a.rows, a.cols);
        size_t i = 0, j = 0;
        // 与两个有序线性表归并一样：小位置先进入结果，相同位置相加。
        while (i < a.terms.size() || j < b.terms.size()) {
            if (j == b.terms.size() || (i < a.terms.size() && Before(a.terms[i], b.terms[j])))
                result.terms.push_back(a.terms[i++]);
            else if (i == a.terms.size() || Before(b.terms[j], a.terms[i]))
                result.terms.push_back(b.terms[j++]);
            else {
                const int value = CheckedInt(1LL * a.terms[i].value + b.terms[j].value);
                if (value != 0) result.terms.push_back({a.terms[i].row, a.terms[i].col, value});
                ++i; ++j; // 相消为0也必须前进，但不保存零项。
            }
        }
        return result;
    }
    static SparseMatrix Multiply(const SparseMatrix& a, const SparseMatrix& b) {
        if (a.cols != b.rows) throw invalid_argument("左矩阵列数必须等于右矩阵行数");
        SparseMatrix result(a.rows, b.cols);
        for (int i = 1; i <= a.rows; ++i)
            for (int j = 1; j <= b.cols; ++j) {
                int sum = 0;
                for (int k = 1; k <= a.cols; ++k)
                    sum = CheckedInt(1LL * sum + 1LL * a.GetElem(i, k) * b.GetElem(k, j));
                if (sum != 0) result.SetElem(i, j, sum);
            }
        return result;
    }
    void PrintTriples() const {
        cout << "(行数,列数,非零元数)=(" << rows << ',' << cols << ',' << terms.size() << ")\n";
        for (const Triple& t : terms) cout << '(' << t.row << ',' << t.col << ',' << t.value << ") ";
        cout << '\n';
    }
    void PrintDense() const {
        for (int i = 1; i <= rows; ++i)
            for (int j = 1; j <= cols; ++j) cout << GetElem(i, j) << (j == cols ? '\n' : ' ');
    }
};

int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    SparseMatrix a(3, 3), b(3, 3);
    a.SetElem(1,1,1); a.SetElem(1,3,3); a.SetElem(2,2,5); a.SetElem(3,1,7);
    b.SetElem(1,1,-1); b.SetElem(1,2,4); b.SetElem(2,2,2); b.SetElem(3,3,6);
    cout << "矩阵A三元组：\n"; a.PrintTriples();
    cout << "GetElem(2,2)=" << a.GetElem(2,2) << '\n';
    a.SetElem(2,2,0);
    cout << "将A(2,2)设为0后：\n"; a.PrintTriples();
    cout << "A转置：\n"; a.Transpose().PrintTriples();
    cout << "A+B：\n"; SparseMatrix::Add(a,b).PrintDense();

    SparseMatrix x(2,3), y(3,2);
    x.SetElem(1,1,1); x.SetElem(1,3,2); x.SetElem(2,2,3);
    y.SetElem(1,1,4); y.SetElem(2,2,5); y.SetElem(3,1,6);
    cout << "X*Y：\n"; SparseMatrix::Multiply(x,y).PrintDense();
    // 此例的int中间结果均可表示；CheckedInt也会拒绝范围外的整数结果。
    cout << "可输入A的行列号查询（1至3；EOF结束）：";
    int r = 0, c = 0;
    if (!(cin >> r)) {
        if (cin.eof()) return 0;
        cout << "错误：行号须为整数。\n"; return 1;
    }
    if (!(cin >> c)) { cout << "错误：请输入两个整数。\n"; return 1; }
    try {
        const int value = a.GetElem(r,c);
        cout << "查询结果：" << value << '\n';
    }
    catch (const exception& e) { cout << "错误：" << e.what() << '\n'; return 1; }
    return 0;
}
