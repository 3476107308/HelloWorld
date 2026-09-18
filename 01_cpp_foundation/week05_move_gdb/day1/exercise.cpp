// ============================================================
// Week05 Day1：GDB 调试基础 — exercise.cpp（独立练习）
// ============================================================
// 本文件是一个有 bug 的程序。你的任务：
//   不要直接看代码猜 bug，而是用 GDB 来定位每一个 bug。
//
// 编译命令（必须加 -g）：
//   g++ -std=c++17 -Wall -g -O0 exercise.cpp -o exercise.exe
//
// 启动 GDB：
//   gdb ./exercise.exe
// ============================================================

#include <iostream>
#include <vector>
#include <string>
#include <windows.h>

using std::cout;
using std::endl;
using std::vector;
using std::string;

// ============================================================
// 需求清单（请逐条完成）
// ============================================================

// -----------------------------------------------------------
// 需求 1：用 GDB 定位程序崩溃位置
// -----------------------------------------------------------
// 下面的 count_positive 函数接收一个 vector<int> 的引用。
// 它有一个 bug——运行会崩溃。
//
// 请用 GDB 完成：
//   ① run 让程序跑起来，崩溃后不要退出 GDB
//   ② 用 backtrace 查看崩溃时的调用栈，记下崩溃的文件和行号
//   ③ 用 frame 0 切换到崩溃的栈帧
//   ④ 用 print 查看相关变量的值
//   ⑤ 把崩溃原因写下来
//   ⑥ 修复 bug，重新编译再运行

int count_positive(const vector<int>& arr)
{
    // TODO: 用 GDB 定位这个函数的 bug
    // 提示：这个循环会越界吗？什么条件出问题？
    int count = 0;
    for (size_t i = 0; i < arr.size(); i++)  // <-- bug 在这里
    {
        if (arr[i] > 0)
        {
            count++;
        }
    }
    return count;
}

void test_count_positive()
{
    cout << "需求 1：count_positive\n";
    vector<int> v = {-3, -1, 0, 5, 8, -2, 10};
    // 正常情况下应该统计到 3 个正数（5, 8, 10）
    int result = count_positive(v);
    cout << "正数个数: " << result << endl;
}

// -----------------------------------------------------------
// 需求 2：观察空指针引发的段错误
// -----------------------------------------------------------
// 下面的 print_string_length 函数有一个明显的 bug：
// 没有检查指针是否为空。
//
// test_null_pointer_bug() 会传入一个空指针，导致段错误。
//
// 请用 GDB 完成：
//   ① 在 print_string_length 函数入口设断点（break 函数名）
//   ② run 运行到断点后，用 print 查看参数 ptr 的值
//   ③ 用 next 单步执行，观察哪一行崩溃
//   ④ 用 backtrace 确认调用路径
//   ⑤ 修复 bug：在函数开头检查 ptr 是否为空
//   ⑥ 重新编译并验证

int print_string_length(const char* ptr)
{
    // TODO: 用 GDB 定位这个函数的 bug
    // 提示：ptr 可能是什么？解引用前应该做什么？
    int len = 0;
    while (ptr[len] != '\0')
    {
        len++;
    }
    cout << "字符串长度: " << len << endl;
    return len;
}

void test_null_pointer_bug()
{
    cout << "\n需求 2：空指针段错误\n";
    const char* p = nullptr;
    print_string_length(p);  // 这里会崩溃
}

// -----------------------------------------------------------
// 需求 3：用 step 进入函数内部，观察逻辑错误
// -----------------------------------------------------------
// 下面的 find_max 函数有一个逻辑错误——它不应该崩溃，
// 但返回的结果不对。
//
// 请用 GDB 完成：
//   ① 在 find_max 函数入口设断点
//   ② 用 next 逐行执行，每次循环后用 print 查看 max_val
//   ③ 找到 max_val 更新错误的根本原因
//   ④ 修复 bug

int find_max(const vector<int>& arr)
{
    // TODO: 用 GDB 找到逻辑 bug
    // 提示：max_val 初始值对吗？如果数组全是负数会怎样？
    if (arr.empty()) return 0;

    int max_val = arr[0];  // <-- bug: 如果全是负数，max_val 应该是数组元素
    for (size_t i = 0; i < arr.size(); i++)
    {
        if (arr[i] > max_val)
        {
            max_val = arr[i];
        }
    }
    return max_val;
}

void test_find_max()
{
    cout << "\n需求 3：find_max 逻辑错误\n";
    vector<int> v1 = {-5, -2, -8, -1, -9};
    // 预期最大值: -1
    // 实际返回值: ?
    int result = find_max(v1);
    cout << "最大值: " << result << "（预期: -1）" << endl;

    vector<int> v2 = {3, 7, 2, 9, 1};
    int result2 = find_max(v2);
    cout << "最大值: " << result2 << "（预期: 9）" << endl;
}

// -----------------------------------------------------------
// 需求 4：练习 display、条件断点
// -----------------------------------------------------------
// 下面的 sum_even_index 函数计算偶数下标元素的和。
// 它没有 bug，但你要用 GDB 的 display 和条件断点来
// 观察它的运行过程。
//
// 请用 GDB 完成：
//   ① 在 sum_even_index 的循环内设断点
//      然后用 display 设置自动显示 i 和 sum
//      每次 continue 观察这两个值的变化
//   ② 删除刚才的断点，重新设一个条件断点：
//      只在 i == 4 时暂停
//   ③ 条件断点触发时，用 info locals 查看所有局部变量
//   ④ 用 ptype 查看 arr 的类型、arr[i] 的类型

int sum_even_index(const vector<int>& arr)
{
    // TODO: 用 display 和条件断点观察这个函数
    int sum = 0;
    for (size_t i = 0; i < arr.size(); i += 2)
    {
        sum += arr[i];
    }
    return sum;
}

void test_sum_even_index()
{
    cout << "\n需求 4：display 和条件断点\n";
    vector<int> v = {10, 20, 30, 40, 50, 60, 70, 80};
    // arr[0] + arr[2] + arr[4] + arr[6] = 10 + 30 + 50 + 70 = 160
    int result = sum_even_index(v);
    cout << "偶数下标和: " << result << "（预期: 160）" << endl;
}

// -----------------------------------------------------------
// 需求 5：用 GDB 调试递归函数
// -----------------------------------------------------------
// fibonacci 函数用递归计算斐波那契数。
//
// 请用 GDB 完成：
//   ① 在 fibonacci 函数入口设断点
//   ② run，每次触发断点时用 backtrace 看调用栈的深度
//   ③ 用 frame N 在不同栈帧之间切换，查看每层 n 的值
//   ④ 调用 fib(3) 时，backtrace 应该显示几层？

int fibonacci(int n)
{
    // TODO: 用 GDB 观察递归调用栈
    if (n <= 1) return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}


void test_fibonacci()
{
    cout << "\n需求 5：递归调用栈\n";
    // 用 fib(3) 来观察，栈不会太深
    int result = fibonacci(3);
    cout << "fib(3) = " << result << "（预期: 2）" << endl;
}

// -----------------------------------------------------------
// 需求 6（综合）：用 GDB 完成今天验收检查表
// -----------------------------------------------------------
// 在终端中执行以下命令，记录每条命令的输出：
//
// [ ] 1. break main           — 设断点
// [ ] 2. run                  — 启动
// [ ] 3. next                 — 单步
// [ ] 4. step                 — 进入函数
// [ ] 5. print 变量            — 查看值
// [ ] 6. display 变量          — 自动显示
// [ ] 7. backtrace            — 调用栈
// [ ] 8. info breakpoints     — 断点列表
// [ ] 9. info locals          — 局部变量
// [ ] 10. quit                — 退出

// -----------------------------------------------------------
// main
// -----------------------------------------------------------

int main()
{
    SetConsoleOutputCP(CP_UTF8);

    cout << "===== Week05 Day1 GDB 练习 =====\n";

    // 需求 1：定位越界崩溃
    test_count_positive();

    // 需求 2：定位空指针段错误
    // 注意：这个函数会崩溃。先用 GDB 定位，修复后再取消注释运行。
    //test_null_pointer_bug();  // ← 修复后取消注释

    // 需求 3：定位逻辑错误
    test_find_max();

    // 需求 4：display 和条件断点
    test_sum_even_index();

    // 需求 5：观察递归调用栈
    test_fibonacci();

    cout << "\n===== 练习完成 =====\n";
    cout << "别忘了用 GDB 跑一遍所有需求，不要只看代码！\n";
    return 0;
}
