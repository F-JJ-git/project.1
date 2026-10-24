// 第5章：三维数组的行优先地址计算。
// 课件：幻灯片8～13（存储顺序和地址公式）、76（作业5.1）。
// A[0:7,0:8,0:9]，基地址1000，每个元素占2个存储单元。
// 输入：i j k；空行或直接结束输入时，使用课件的(1,6,8)。
// 复杂度：给定三维数组的地址和总空间均为O(1)时间、O(1)辅助空间。
#include <iostream>
#include <sstream>
#include <string>
#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif
using namespace std;

int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    const long long base = 1000, elementSize = 2;
    const int b1 = 8, b2 = 9, b3 = 10;
    int i = 1, j = 6, k = 8;
    string line;
    cout << "输入 i j k（空行使用课件的1 6 8）：";
    if (getline(cin, line) && line.find_first_not_of(" \t\r") != string::npos) {
        istringstream in(line);
        char extra;
        if (!(in >> i >> j >> k) || (in >> extra)) {
            cout << "错误：请输入三个整数下标。\n";
            return 1;
        }
    }
    if (i < 0 || i >= b1 || j < 0 || j >= b2 || k < 0 || k >= b3) {
        cout << "下标越界：i在0至7，j在0至8，k在0至9。\n";
        return 1;
    }

    // 最右侧下标k变化最快。先越过i个二维平面，再越过j行，最后偏移k个元素。
    const long long offset = (1LL * i * b2 + j) * b3 + k;
    const long long address = base + offset * elementSize;
    const long long elementCount = 1LL * b1 * b2 * b3;
    cout << "A[" << i << "][" << j << "][" << k << "] 的起始地址：" << address << '\n';
    cout << "偏移元素个数：" << offset << '\n';
    cout << "数组元素个数：" << elementCount << '\n';
    cout << "数组总存储空间：" << elementCount * elementSize << "（存储单元）\n";
    // 元素个数720与总空间1440不同；若每个存储单元为1字节，总空间才可写为1440字节。
    return 0;
}
