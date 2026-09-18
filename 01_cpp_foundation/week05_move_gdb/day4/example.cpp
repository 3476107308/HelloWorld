// ============================================================
// Week05 Day4：nullptr + using 别名 + 右值引用基础 — example.cpp（教学示例）
// ============================================================
// 编译：
//   g++ -std=c++17 -Wall -g -O0 example.cpp -o example.exe
// 调试：
//   gdb ./example.exe
//
// 补充视频：大丙 C++11 新特性详解「nullptr/using」「第三部分：右值引用基础」
//   https://space.bilibili.com/147020887/lists/6687062
// ============================================================

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <windows.h>

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::unordered_map;

// ============================================================
// 第 1 节：nullptr — 为什么不要用 NULL
// ============================================================
// NULL 在 C++ 里本质是 0（或 0L），不是指针类型。
// 这导致函数重载时会出现诡异行为。
// nullptr 是 C++11 引入的真正的"空指针"类型 std::nullptr_t。
// ============================================================

void f(int x)
{
    cout << "调用了 f(int): " << x << endl;
}

void f(void* p)
{
    cout << "调用了 f(void*): " << (p ? "非空" : "空指针") << endl;
}

void lesson1_nullptr_vs_NULL()
{
    cout << "\n========== 第 1 节：nullptr vs NULL ==========\n";

    // NULL 的本质：是一个宏，展开为 0
    // #define NULL 0        （C++ 标准写法）
    // #define NULL ((void*)0) （C 标准写法，C++ 不用这个）

    f(42);       // 调用 f(int)，毫无疑问

    f(0);     // 你以为调 f(void*)？实际上调了 f(int)！
    // NULL 就是 0，所以 f(NULL) 等价于 f(0)，结果调了 f(int)
    // 你的编译器甚至直接报二义性错误——连编译都过不了！

    f(nullptr);  // 明确调用 f(void*)——nullptr 的类型是 std::nullptr_t
    // 可以隐式转为任意指针类型，但不是 int

    // ★ 面试经典题：NULL 和 nullptr 的区别
    //
    // NULL：
    //   - 是宏，定义为 0
    //   - 类型是 int（或 long）
    //   - 重载解析时会被当成整数
    //
    // nullptr：
    //   - 是关键字，类型为 std::nullptr_t
    //   - 只能转为指针类型，永远不会被当成 int
    //   - 重载解析行为符合直觉

    // 验证 nullptr 的类型
    auto np = nullptr;                      // np 是 std::nullptr_t
    cout << "nullptr 类型: "
         << (std::is_same<decltype(np), std::nullptr_t>::value
             ? "std::nullptr_t" : "???") << endl;

    // 可以赋值给任意指针
    int* p1 = nullptr;
    double* p2 = nullptr;
    void (*fp)() = nullptr;        // 函数指针也行
    cout << "nullptr 赋值给 int*、double*、函数指针都 OK\n";
}

// ============================================================
// 第 2 节：using 别名 —— 比 typedef 更好用
// ============================================================
// using 可以做 typedef 的所有事，而且语法更直观：
//   typedef 旧类型 新名字;     → 类型名在最后，复杂时常人看不懂
//   using 新名字 = 旧类型;     → 赋值式语法，直觉更易读
//
// 更重要的是：using 支持模板别名，typedef 做不到。
// ============================================================

// ---- typedef vs using ----

// 函数指针：typedef 版本
typedef void(*FuncPtr1)(int, double);     // FuncPtr1 藏在中间，不好找
// 函数指针：using 版本
using FuncPtr2 = void(*)(int, double);    // 一眼看出 FuncPtr2 是函数指针

// 数组指针：typedef 版本
typedef int(*ArrPtr1)[10];                // 解读费劲
// 数组指针：using 版本
using ArrPtr2 = int(*)[10];               // 清晰

// ---- 模板别名：using 的杀手锏，typedef 做不到 ----

// 给 unordered_map<string, vector<int>> 起个短名
using IndexMap = unordered_map<string, vector<int>>;

// 模板别名：给任何类型的 vector 起通用短名
template<typename T>
using Vec = vector<T>;

// typedef 无法写模板别名——它是用"声明变量"的方式定义类型，
// 没有位置放 template<>

void lesson2_using_alias()
{
    cout << "\n========== 第 2 节：using 别名 ==========\n";

    // 用模板别名——Vec<T> 等价于 vector<T>
    Vec<int> vi = {1, 2, 3};
    Vec<string> vs = {"hello", "world"};
    cout << "Vec<int> 等价于 vector<int>: " << vi[0] << endl;
    cout << "Vec<string> 等价于 vector<string>: " << vs[0] << endl;

    // 用 IndexMap——省掉长类型名
    IndexMap m;
    m["张三"] = {85, 92, 78};
    m["李四"] = {90, 88, 95};
    cout << "IndexMap 等价于 unordered_map<string, vector<int>>: "
         << m["张三"][0] << endl;

    // 没有 using 的话，上面三行要这样写：
    // unordered_map<string, vector<int>> m;
    // m["张三"] = {85, 92, 78};
    // m["李四"] = {90, 88, 95};
    // ——每次都要重复长类型名，又丑又易错
}

// ============================================================
// 第 3 节：左值和右值 —— 判断方法
// ============================================================
// 左值（lvalue）：有名字、有地址、表达式结束后还存在。
// 右值（rvalue）：没有名字、临时对象、表达式结束后就销毁。
//
// 快速判断口诀：
//   - 能取地址(&)的就是左值
//   - 不能取地址的就是右值
//
// 常见右值：
//   - 字面量：42, 3.14, "hello"
//   - 临时对象：string("abc"), vector<int>{1,2,3}
//   - 函数返回非引用类型：int foo() 返回的结果
//   - std::move(x) 的结果
// ============================================================

// 辅助函数：返回临时对象（右值）
string make_string()
{
    return string("临时字符串");
}

// 辅助函数：返回引用（左值）
string global_str = "全局字符串";
string& get_ref()
{
    return global_str;
}

// 判断左值右值的宏（编译期）
// 在 GDB 中，你也可以用 ptype 验证

void lesson3_lvalue_rvalue()
{
    cout << "\n========== 第 3 节：左值 vs 右值 ==========\n";

    int x = 42;              // x 是左值（有名字，有地址）
    // &x;                   // OK，可以取地址 → 左值
    // &42;                  // 报错！不能取地址 → 右值

    string s = "hello";      // s 是左值
    // "hello"               // 字面量是右值
    // s + " world"          // 表达式结果是临时 string → 右值

    string t = make_string();// make_string() 返回临时对象 → 右值
    // t 是左值（有名字了）

    string& r = get_ref();   // get_ref() 返回引用 → 左值

    cout << "判断口诀：能取地址 &x → 左值；不能取地址 → 右值\n";
    cout << "int x=42;  // x 是左值\n";
    cout << "42;        // 字面量是右值\n";
    cout << "s + \" w\";   // 表达式结果是右值\n";
    cout << "make_string() // 返回临时对象是右值\n";
    cout << "get_ref()     // 返回引用是左值\n";

    // ---- 重要：std::move(x) 是右值 ----
    int a = 10;
    // a 本身是左值
    // std::move(a) 返回 int&&（右值引用 → 右值）
    cout << "\nstd::move(a) 将左值 a 转换为右值\n";
    // 注意：move 不移动数据！只是类型转换（cast 成右值引用）

    // ---- 重要悖论：右值引用类型的变量本身是左值！ ----
    int&& rr = 42;           // rr 绑定到右值 42
    // rr 本身有名字、有地址 → rr 是左值！
    // &rr;                   // OK，可以取地址
    cout << "int&& rr = 42;  // rr 本身是左值（有名字、有地址）\n";
    cout << "  右值引用类型的变量，自己却是左值——这是最反直觉的一点\n";
}

// ============================================================
// 第 4 节：右值引用 T&& —— 作用
// ============================================================
// T&& 的三大用途：
//   1. 绑定到临时对象，延长其生命周期
//   2. 区分重载——左值走 T& 版本，右值走 T&& 版本
//   3. 实现移动语义（Day 5 详细讲）
//
// 一个特例：const T& 也能绑到临时对象，但它不能修改，且不能区分左右值。
// ============================================================

void test_overload(int& x)
{
    cout << "  → 调用了 int& 版本（左值）\n";
}

void test_overload(const int& x)
{
    cout << "  → 调用了 const int& 版本（只读，可绑右值）\n";
}

void test_overload(int&& x)
{
    cout << "  → 调用了 int&& 版本（右值！）\n";
}

void lesson4_rvalue_ref()
{
    cout << "\n========== 第 4 节：右值引用 ==========\n";

    // ---- 用途 1：绑定到临时对象 ----
    int&& r1 = 100;           // 右值引用绑定到字面量 100
    cout << "int&& r1 = 100;  r1 = " << r1 << endl;

    // 可以修改 r1（虽然它原来绑的是右值，但 r1 本身是左值）
    r1 = 200;
    cout << "r1 = 200;  现在 r1 = " << r1 << endl;

    // const T& 也能绑定临时对象，但不能修改
    const int& r2 = 100;
    cout << "const int& r2 = 100;  // 也能绑，但不能修改\n";

    // ---- 用途 2：区分重载（左值右值走不同函数） ----
    int a = 10;
    cout << "\ntest_overload(a):     ";  test_overload(a);          // int&
    cout << "test_overload(42):    ";  test_overload(42);         // int&&
    cout << "test_overload(move(a)):"; test_overload(std::move(a));// int&&

    // 重载优先级：
    //   左值 → 优先 int&，其次 const int&
    //   右值 → 优先 int&&，其次 const int&
    //   没有 const int& → 右值传不进去（int& 不接受右值）

    // ---- GDB 练习 ----
    // 在 test_overload(int&&) 版本设断点，传不同参数进去，观察调用
    // (gdb) break test_overload
    // (gdb) run
    // (gdb) bt  → 看是从哪个调用进来的
}

// ============================================================
// 第 5 节：用 GDB 观察右值引用
// ============================================================
// 在右值引用参数函数设断点，看地址和值
// ============================================================

struct Data
{
    string content;
    Data(string c) : content(c)
    {
        cout << "[Data] 创建: " << content << endl;
    }
    ~Data()
    {
        cout << "[Data] 销毁: " << content << endl;
    }
};

void eat_data(Data& d)         { cout << "  吃左值: " << d.content << endl; }
void eat_data(const Data& d)   { cout << "  吃只读: " << d.content << endl; }
void eat_data(Data&& d)        { cout << "  吃右值: " << d.content << endl; }

void lesson5_gdb_practice()
{
    cout << "\n========== 第 5 节：GDB 观察右值引用 ==========\n";

    Data d("左值对象");
    // (gdb) break eat_data
    // (gdb) run
    // (gdb) next   ← 观察每次走哪个重载

    cout << "\n传左值:";
    eat_data(d);                  // → Data& 版本

    cout << "传临时对象:";
    eat_data(Data("右值临时"));    // → Data&& 版本

    cout << "传 move(d):";
    eat_data(std::move(d));       // → Data&& 版本（d 被转成右值）
    // 注意：move 之后 d 仍然存在，但内容已被"搬走"的语义暗示
    // 实际 Data 没有移动构造，所以这里不会真搬
}

// -----------------------------------------------------------
// main
// -----------------------------------------------------------

int main()
{
    SetConsoleOutputCP(CP_UTF8);

    lesson1_nullptr_vs_NULL();
    lesson2_using_alias();
    lesson3_lvalue_rvalue();
    lesson4_rvalue_ref();
    lesson5_gdb_practice();

    cout << "\n========== 程序正常结束 ==========\n";
    return 0;
}

// ============================================================
// 今日要点总结
// ============================================================
//
// nullptr vs NULL:
//   NULL = 0（int）         → 重载时可能调到 int 版本
//   nullptr = 空指针字面量   → 只能转指针，重载行为符合直觉
//
// using vs typedef:
//   using 类型名 = 原类型;         → 更直观
//   template<T> using Vec = ...   → 模板别名（typedef 做不到）
//
// 左值 vs 右值:
//   左值：有名字、有地址、表达式结束后还存在
//   右值：没有名字、临时对象、表达式结束后就销毁
//   判断口诀：能 &取地址的是左值，不能的是右值
//
// T&& 右值引用:
//   1. 绑定临时对象，延长生命周期
//   2. 区分重载（左值走 T&，右值走 T&&）
//   3. 实现移动语义（Day 5）
//   ★ 陷阱：右值引用变量本身是左值！
// ============================================================
