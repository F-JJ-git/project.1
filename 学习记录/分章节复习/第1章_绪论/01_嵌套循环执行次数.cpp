/*
 * 第1章：嵌套循环执行次数
 * 课件对应：第41～50张讲时间频度与复杂度，第56张作业1.3给出本题循环。
 * 目标：既实际统计 x = x - 1 的执行次数，也用求和公式核对。
 * 输入：一个整数 n（演示范围1～10000）；输出：实际次数、公式值、最终x。
 * 核心：内层次数依赖外层i，不能把两层循环都当成执行n次。
 * 时间：实际模拟Theta(n^2)，公式计算O(1)；额外空间O(1)。
 * 演示上限是为了避免等待过久，不是数学公式成立的上限。
 */
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
    constexpr int MAX_SIMULATED_N = 10000;
    cout << "输入 n（1至" << MAX_SIMULATED_N << "）：";
    string line;
    int n = 0;
    char extra = '\0';
    if (!getline(cin, line)) {
        cout << "未读取到输入。\n";
        return 1;
    }
    istringstream input(line);
    if (!(input >> n) || (input >> extra) || n < 1 || n > MAX_SIMULATED_N) {
        cout << "请输入1至10000之间的整数。\n";
        return 1;
    }

    long long x = 0;
    long long count = 0;
    for (int i = 1; i <= n - 1; ++i) {
        // 固定i时，j从i+1到n（含端点），共执行n-i次。
        // 每轮开始时，count等于此前所有内层循环已执行的总次数。
        for (int j = i + 1; j <= n; ++j) {
            x = x - 1;
            ++count;
        }
    }

    // 总次数=(n-1)+(n-2)+...+1=n(n-1)/2。
    // 1LL让乘法先按long long进行，避免先用int相乘再转换。
    const long long theoretical = 1LL * n * (n - 1) / 2;
    cout << "语句 x = x - 1 的实际执行次数：" << count << '\n';
    cout << "理论公式 n(n-1)/2：" << theoretical << '\n';
    cout << "最终 x：" << x << '\n';
    cout << "次数核对：" << (count == theoretical ? "一致" : "不一致") << '\n';
    cout << "模拟时间复杂度：O(n^2)，辅助空间复杂度：O(1)\n";
    return 0;
}
