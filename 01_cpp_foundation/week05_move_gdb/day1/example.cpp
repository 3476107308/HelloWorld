// ============================================================
// Week05 Day1：GDB 调试基础 — example.cpp（教学示例）
// ============================================================
// 本文件是一个正常的 C++ 程序，配合 GDB 学习断点、单步、
// 查看变量、调用栈等基本调试命令。
//
// 编译命令（必须加 -g 生成调试信息，-O0 关闭优化）：
//   g++ -std=c++17 -Wall -g -O0 example.cpp -o example.exe
//
// 启动 GDB：
//   gdb ./example.exe
//
// 补充视频：simplesoft GDB 调试合集
//   https://space.bilibili.com/391667478/lists/8218249
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
// 第 1 节：GDB 环境准备
// ============================================================
// 在开始之前，确认你的环境：
//   Windows + MSYS2: pacman -S mingw-w64-ucrt-x86_64-gdb
//   WSL2 + Ubuntu:   sudo apt install gdb
//
// 编译后启动 GDB：
//   $ gdb ./example.exe
//   (gdb) break main       ← 在 main 函数设断点
//   (gdb) run              ← 运行程序
//   (gdb) next             ← 单步执行（不进入函数内部）
//   (gdb) step             ← 单步执行（进入函数内部）
//   (gdb) print 变量名      ← 查看变量值
//   (gdb) backtrace        ← 查看调用栈（缩写 bt）
//   (gdb) quit             ← 退出 GDB（缩写 q）
// ============================================================

// -----------------------------------------------------------
// 几个供 GDB 练习使用的简单函数
// -----------------------------------------------------------

// 计算数组元素和——你可以在这里设断点，观察 sum 的变化
int array_sum(const vector<int>& arr)
{
    int sum = 0;                          // (gdb) break array_sum
    for (size_t i = 0; i < arr.size(); i++)
    {
        sum += arr[i];                    // (gdb) print sum   ← 每次暂停看 sum
    }                                     // (gdb) display sum ← 自动显示 sum
    return sum;
}

// 交换两个 int——观察参数是引用，直接修改原变量
void swap_int(int& a, int& b)
{
    int temp = a;                         // (gdb) step ← step 会进入这行
    a = b;                                // (gdb) print a
    b = temp;                             // (gdb) print b
}                                         // (gdb) finish ← 执行完当前函数返回

// 计算阶乘（递归版）——练习查看调用栈
// 当 n=5 时，递归调用 5 层，用 backtrace 可以看到所有调用帧
long long factorial(int n)
{
    if (n <= 1) return 1;                 // 基准条件：在这里设断点
    return n * factorial(n - 1);          // 递归调用
}

// 一个结构体——练习 print 复杂类型
struct Point
{
    int x;
    int y;
    string name;
};

// 故意能触发段错误的函数——练习用 backtrace 定位崩溃（见第 4 节）
void dangerous_function()
{
    int* p = nullptr;                     // 空指针
    // *p = 42;                           // 取消注释就会段错误！
}

void print_point(const Point& pt)
{
    cout << pt.name << ": (" << pt.x << ", " << pt.y << ")\n";
}

// -----------------------------------------------------------
// 第 2 节：基本 GDB 命令练习
// -----------------------------------------------------------
// 下面的函数每一步都标注了对应的 GDB 命令。
// 请一边读注释，一边在 GDB 里实际操作。

void lesson1_gdb_basics()
{
    cout << "\n========== 第 2 节：基本 GDB 命令 ==========\n";

    // ---- 练习 break + run ----
    // (gdb) break lesson1_gdb_basics   ← 在这个函数入口设断点
    // 或者：
    // (gdb) break example.cpp:77      ← 在这一行设断点

    int a = 10;
    int b = 20;
    // (gdb) next  ← 执行下一行（不进入函数内部）
    // (gdb) print a  → 输出 10
    // (gdb) print b  → 输出 20

    // ---- 练习 step vs next ----
    swap_int(a, b);
    // 在这一行：
    //   (gdb) next  → 跳过 swap_int 内部，直接得到结果
    //   (gdb) step  → 进入 swap_int 函数内部，逐行执行
    //
    // 试试两种操作，体会区别！

    cout << "交换后: a=" << a << " b=" << b << endl;
    // 应该是 a=20, b=10

    // ---- 练习 print 复杂类型 ----
    vector<int> v = {1, 2, 3, 4, 5};
    // (gdb) print v        → 输出 vector 的内部结构（不直观）
    // (gdb) print v.size() → 输出 5  ← print 可以调用成员函数！

    int result = array_sum(v);
    // (gdb) print result → 输出 15
    cout << "数组和: " << result << endl;

    // ---- 练习 print 指针 ----
    int x = 42;
    int* ptr = &x;
    // (gdb) print ptr   → 输出地址 0x...
    // (gdb) print *ptr  → 输出 42（解引用）
    // (gdb) print &x    → 输出 x 的地址（应该和 ptr 相同）

    // ---- 练习 display（自动显示） ----
    // (gdb) display x     ← 设置后每次暂停自动显示 x 的值
    // (gdb) display *ptr  ← 自动显示 ptr 指向的内容
    // (gdb) info display  ← 查看所有 display 设置
    // (gdb) undisplay 1   ← 取消编号为 1 的 display
}

// -----------------------------------------------------------
// 第 3 节：backtrace 和调用栈
// -----------------------------------------------------------

void level3(int n)
{
    // (gdb) break level3       ← 在这里设断点
    // (gdb) backtrace          ← 查看完整调用栈
    // (gdb) bt                 ← 缩写
    //
    // 输出类似：
    //   #0  level3 (n=1) at example.cpp:...
    //   #1  level2 (n=2) at example.cpp:...
    //   #2  level1 (n=3) at example.cpp:...
    //   #3  lesson2_callstack () at example.cpp:...
    //   #4  main () at example.cpp:...
    //
    // 最上面 #0 是当前函数，越往下越接近 main
    //
    // (gdb) frame 2            ← 切换到 #2 栈帧（level1 的上下文）
    // (gdb) print n            ← 看到 level1 的 n 是 3
    // (gdb) frame 0            ← 切回当前
    //
    // 这个能力在排查崩溃时极其重要：
    //   程序挂了 → bt 看调用栈 → frame N 查看每一层的变量
    //   → 就能知道崩溃前发生了什么

    long long f = factorial(n);
    cout << n << "! = " << f << endl;
}

void level2(int n)
{
    level3(n - 1);
}

void level1(int n)
{
    level2(n - 1);
}

void lesson2_callstack()
{
    cout << "\n========== 第 3 节：backtrace 和调用栈 ==========\n";
    level1(5);  // 5 → 4 → 3 → 2 → 1 → 递归计算 factorial
}

// -----------------------------------------------------------
// 第 4 节：用 GDB 定位段错误（重点！）
// -----------------------------------------------------------

// 一个制造了越界访问的函数——用来练习定位段错误
void buggy_out_of_bounds()
{
    cout << "\n========== 第 4 节：定位段错误 ==========\n";
    cout << "运行 buggy 函数...\n";

    int arr[5] = {10, 20, 30, 40, 50};
    // 故意越界——arr 只有 5 个元素 (0-4)，访问 [100] 会段错误
    // (gdb) run   ← 程序会在这里崩溃
    // (gdb) bt    ← 立即看调用栈，GDB 会告诉你崩溃在哪一行！
    // (gdb) print i  ← 查看越界的索引值
    // (gdb) frame 0  ← 确认崩溃位置

    for (int i = 0; i < 100; i++)
    {
        arr[i] = i;  // i=5 开始就越界了
    }
}

// 另一个常见 bug：访问已释放的内存（悬垂指针）
int* create_dangling()
{
    int local = 999;
    // 错误！返回了局部变量的地址
    // 函数返回后 local 已经被销毁，ptr 指向的是"悬垂"内存
    // return &local;  // 这行故意注释掉，因为编译器会 warning
    return nullptr;     // 安全返回，实际练习见 exercise.cpp
}

// -----------------------------------------------------------
// 第 5 节：GDB 实用技巧总结
// -----------------------------------------------------------

void lesson3_gdb_tips()
{
    cout << "\n========== 第 5 节：GDB 实用技巧 ==========\n";

    // 技巧 1：条件断点
    // (gdb) break lesson3_gdb_tips if false   ← 条件不满足，断点不触发
    // (gdb) break 某行号 if i == 50          ← 只在 i=50 时暂停
    // 在循环里调试特定迭代时非常有用

    // 技巧 2：查看数组
    int arr[10] = {0, 1, 4, 9, 16, 25, 36, 49, 64, 81};
    // (gdb) print arr         → 只显示地址
    // (gdb) print *arr@10     → 显示 arr[0] 到 arr[9] 共 10 个元素
    // 格式：print *指针@元素个数

    // 技巧 3：查看类型
    auto x = 3.14;           // auto 推导为 double
    vector<int> v = {1,2,3};
    auto it = v.begin();     // auto 推导为 vector<int>::iterator
    // (gdb) ptype x          → 输出 double
    // (gdb) ptype it         → 输出迭代器的完整类型

    // 技巧 4：修改变量值
    // (gdb) set var x = 100  ← 在 GDB 中修改 x 的值
    // (gdb) continue          ← 继续运行，程序用的是你修改后的值
    // 用于测试"如果这里是 xx，程序会怎样？"而不用重新编译

    // 技巧 5：断点管理
    // (gdb) info breakpoints  ← 查看所有断点（缩写 info b）
    // (gdb) disable 1         ← 临时禁用 1 号断点
    // (gdb) enable 1          ← 重新启用
    // (gdb) delete 1          ← 删除 1 号断点
    // (gdb) delete            ← 删除所有断点
}

// -----------------------------------------------------------
// main
// -----------------------------------------------------------

int main()
{
    SetConsoleOutputCP(CP_UTF8);

    // ---- 请按照顺序在 GDB 中操作 ----
    //
    // 第一步：先体验基本命令
    lesson1_gdb_basics();
    //
    // 第二步：练习查看调用栈
    lesson2_callstack();
    //
    // 第三步：学习实用技巧
    lesson3_gdb_tips();
    //
    // 第四步（可选）：取消注释下面这行，在 GDB 中练习定位段错误
    buggy_out_of_bounds();

    cout << "\n========== 程序正常结束 ==========\n";
    return 0;
}

// ============================================================
// GDB 命令速查表（建议打印或手抄贴在屏幕旁边）
// ============================================================
//
// ┌─────────────────────────────────────────────────────────┐
// │ 命令 (缩写)           │ 作用                             │
// ├─────────────────────────────────────────────────────────┤
// │ break 函数名 (b)      │ 在函数入口设断点                  │
// │ break 行号            │ 在指定行设断点                    │
// │ break 行号 if 条件    │ 条件断点                          │
// │ run (r)              │ 从头运行程序                       │
// │ next (n)             │ 单步执行，不进入函数内部            │
// │ step (s)             │ 单步执行，进入函数内部              │
// │ finish               │ 执行到当前函数返回                 │
// │ continue (c)         │ 继续运行到下一个断点                │
// │ print 变量 (p)       │ 打印变量值                         │
// │ print *ptr@N         │ 打印指针指向的 N 个元素             │
// │ display 变量          │ 每次暂停自动显示变量               │
// │ backtrace (bt)       │ 显示调用栈                         │
// │ frame N              │ 切换到第 N 层栈帧                  │
// │ ptype 变量            │ 打印变量的类型                     │
// │ info breakpoints     │ 查看所有断点                       │
// │ info locals          │ 查看当前栈帧的所有局部变量          │
// │ list (l)             │ 显示当前位置的源码                 │
// │ set var 变量=值       │ 在 GDB 中修改变量值                │
// │ quit (q)             │ 退出 GDB                          │
// └─────────────────────────────────────────────────────────┘
//
// 今天的目标：把这 16 个命令在 example.cpp 上全部操作一遍。
// 明天开始用 GDB 调试你自己写的 exercise.cpp。
// ============================================================
