// ============================================================
// Week05 Day3：decltype + 返回类型后置 — example.cpp（教学示例）
// ============================================================
// 编译：
//   g++ -std=c++17 -Wall -g -O0 example.cpp -o example.exe
// 调试：
//   gdb ./example.exe
//
// 补充视频：大丙 C++11 新特性详解「第二部分：decltype + 返回类型后置」
//   https://space.bilibili.com/147020887/lists/6687062
// ============================================================

#include <iostream>
#include <vector>
#include <string>
#include <type_traits>
#include <windows.h>

using std::cout;
using std::endl;
using std::vector;
using std::string;

// ============================================================
// 第 1 节：decltype 基础 —— 变量 vs 表达式
// ============================================================
// decltype 有两条规则，记住就行：
//
// 规则 A：decltype(变量名) → 变量的声明类型（保留顶层 const 和引用）
// 规则 B：decltype(表达式) → 表达式的值类别决定：
//          - 左值表达式 → T&（引用）
//          - 将亡值(xvalue) → T&&（右值引用）
//          - 纯右值 → T（值类型）
//
// 关键区别：decltype(变量) 是看声明，decltype(表达式) 是看值类别。
// ============================================================

void lesson1_decltype_basics()
{
    cout << "\n========== 第 1 节：decltype 基础 ==========\n";

    // -------------------------------------------------------
    // 规则 A：decltype(变量名) → 变量的声明类型
    // -------------------------------------------------------
    {
        int x = 42;
        const int cx = 100;
        int& ref = x;
        const int& cref = x;

        // decltype(变量名)：原封不动保留声明类型
        decltype(x)    a = 0;        // a 是 int
        decltype(cx)   b = 0;        // b 是 const int
        decltype(ref)  c = x;        // c 是 int&（保留了引用！）
        decltype(cref) d = x;        // d 是 const int&

        cout << "decltype(x):    int        → a 是 int\n";
        cout << "decltype(cx):   const int  → b 是 const int\n";
        cout << "decltype(ref):  int&       → c 是 int&（保留引用）\n";
        cout << "decltype(cref): const int& → d 是 const int&\n";
        cout << "  验证: " << (std::is_same<decltype(c), int&>::value ? "c 是 int& ✓" : "✗") << endl;

        // 对比 auto：auto 会丢掉引用和顶层 const
        auto a2 = ref;               // a2 是 int（丢了引用！）
        cout << "  对比: auto a2 = ref; → a2 是 int（丢了引用）\n";
    }

    // -------------------------------------------------------
    // 规则 B：decltype(表达式) → 看值类别
    // -------------------------------------------------------
    {
        int x = 42;

        // x 是左值 → 左值表达式返回 T&
        decltype((x))    e = x;      // e 是 int&（双括号！永远引用）
        decltype(x + 0)  f = 0;      // f 是 int（x+0 是纯右值）
        decltype(x = 1)  g = x;      // g 是 int&（赋值表达式返回左值）

        // std::move(x) 是 xvalue（将亡值） → T&&
        decltype(std::move(x)) h = std::move(x); // h 是 int&&

        cout << "\ndecltype((x)):            int&   → 双括号永远是引用\n";
        cout << "decltype(x + 0):          int    → 算术结果是纯右值\n";
        cout << "decltype(std::move(x)):   int&&  → move 返回将亡值\n";
    }

    // -------------------------------------------------------
    // ★ 重点：decltype(x) vs decltype((x))
    // -------------------------------------------------------
    // 这是面试最爱考的！
    {
        int x = 10;
        decltype(x)  t1 = x;    // t1 是 int（规则 A：变量名）
        decltype((x)) t2 = x;   // t2 是 int&（规则 B：(x) 是表达式，左值 → int&）

        cout << "\n★ 核心：decltype(x) vs decltype((x))\n";
        cout << "decltype(x)  → int  （变量名，原封不动）\n";
        cout << "decltype((x)) → int& （表达式，x 是左值 → 引用）\n";
        // 就多了一对括号，类型完全变了——面试必问
    }
}

// ============================================================
// 第 2 节：返回类型后置（trailing return type）
// ============================================================
// 语法：auto 函数名(参数) -> 返回类型 { 函数体 }
//
// 什么时候必须用？
//   模板函数的返回类型依赖模板参数时，你写不出来类型名。
//   因为编译器读到参数列表才知道 T 和 U 是什么。
// ============================================================

// -------------------------------------------------------
// 场景 1：不用返回类型后置 —— 只能写死返回类型
// -------------------------------------------------------
int add_int(int a, int b)
{
    return a + b;
}
// 问题是：int + double 时返回类型写什么？写 int 会截断，写 double 对 int+int 又浪费。

// -------------------------------------------------------
// 场景 2：用 decltype + 返回类型后置 —— 完美推导
// -------------------------------------------------------
// 注意 auto 放前面，箭头 -> decltype(...) 放后面
template<typename T, typename U>
auto add(T a, U b) -> decltype(a + b)
{
    return a + b;
}
// decltype(a+b) 在参数列表之后，编译器已经知道 T 和 U 了，
// 可以正确推导出 a+b 的类型。

// -------------------------------------------------------
// 场景 3：更复杂的例子——模板函数返回容器元素
// -------------------------------------------------------
// 返回 vector 的第一个元素（引用），但不知道 T 是什么
template<typename Container>
auto first_element(Container& c) -> decltype(c[0])
{
    return c[0];  // 返回类型和 c[0] 一样（通常是 T&）
}

// -------------------------------------------------------
// 场景 4：decltype(auto) —— C++14 简写
// -------------------------------------------------------
// C++14 起可以用 decltype(auto) 替代 auto + -> decltype(...)
template<typename T, typename U>
decltype(auto) add_c14(T a, U b)     // 等价于 auto ... -> decltype(a+b)
{
    return a + b;
}
// decltype(auto) 告诉编译器：用 decltype 的规则推导返回类型，保留引用和 const。
// 对比：auto 返回会丢引用，decltype(auto) 不会。

void lesson2_trailing_return()
{
    cout << "\n========== 第 2 节：返回类型后置 ==========\n";

    auto r1 = add(1, 2);           // int + int → int
    auto r2 = add(1, 2.5);         // int + double → double
    auto r3 = add(1.5, 2.5);       // double + double → double

    cout << "add(1, 2)     = " << r1 << "，类型: "
         << (std::is_same<decltype(r1), int>::value ? "int" : "???") << endl;
    cout << "add(1, 2.5)   = " << r2 << "，类型: "
         << (std::is_same<decltype(r2), double>::value ? "double" : "???") << endl;
    cout << "add(1.5, 2.5) = " << r3 << "，类型: "
         << (std::is_same<decltype(r3), double>::value ? "double" : "???") << endl;

    // first_element 测试
    vector<string> names = {"Alice", "Bob", "Charlie"};
    auto& name = first_element(names);  // 返回 string&，不需要拷贝
    // 如果不返回引用，这里 auto 会拷贝整个 string
    name = "Alex";                      // 直接修改 vector 中的元素
    cout << "first_element 修改后 names[0] = " << names[0] << endl;

    // decltype(auto) vs auto 的区别：一个有引用，一个没有
    int x = 42;
    auto            v1 = [&]() { return x; }();      // auto 丢了引用 → int
    decltype(auto)  v2 = [&]() { return x; }();      // decltype(auto) 保留引用 → int&
    // v1 = 100;  // OK，v1 是 int，改了也不影响 x
    v2 = 100;     // 改了 v2 就是改 x！
    cout << "auto 返回 x: 不影响原变量\n";
    cout << "decltype(auto) 返回 x 后 x = " << x << "（被 v2 改了）\n";
}

// ============================================================
// 第 3 节：decltype 的实际应用
// ============================================================

void lesson3_decltype_use_cases()
{
    cout << "\n========== 第 3 节：decltype 实际应用 ==========\n";

    // -------------------------------------------------------
    // 应用 1：声明"和某个已有变量类型相同"的新变量
    // -------------------------------------------------------
    {
        vector<int> v = {1, 2, 3};
        // 我想用迭代器类型，又不想写一长串
        decltype(v)::iterator it = v.begin();  // 等价于 vector<int>::iterator

        cout << "用 decltype(v)::iterator 声明的迭代器: " << *it << endl;
    }

    // -------------------------------------------------------
    // 应用 2：配合模板——从表达式推导类型
    // -------------------------------------------------------
    {
        auto multiply = [](auto a, auto b) -> decltype(a * b)  // C++14 泛型 lambda
        {
            return a * b;
        };
        cout << "multiply(3, 4.5) = " << multiply(3, 4.5) << "（int * double → double）\n";
    }

    // -------------------------------------------------------
    // 应用 3：decltype 与 std::declval —— 不构造对象也能推导类型
    // -------------------------------------------------------
    {
        // 如果类型没有默认构造函数，怎么推导 a+b 的类型？
        // 用 std::declval<T>() —— 假装有一个 T 对象，但不实际构造它
        // 这样就能在编译期推导出 a+b 的类型，无需真正调用构造函数
        using result_t = decltype(std::declval<int>() + std::declval<double>());
        cout << "declval<int>() + declval<double>() 的类型: double\n";
        cout << "  验证: " << (std::is_same<result_t, double>::value ? "double ✓" : "✗") << endl;
    }
}

// ============================================================
// 第 4 节：用 GDB ptype 验证 decltype 推导
// ============================================================

void lesson4_gdb_practice()
{
    cout << "\n========== 第 4 节：GDB ptype 练习 ==========\n";

    int x = 10;
    const int cx = 100;
    int& ref = x;

    // 请用 GDB ptype 逐一验证以下变量的类型
    // (gdb) break lesson4_gdb_practice
    // (gdb) run
    // (gdb) next ← 每走一步 ptype 验证一个

    decltype(x)      d1 = 0;     // (gdb) ptype d1 → int
    decltype(cx)     d2 = 0;     // (gdb) ptype d2 → const int
    decltype(ref)    d3 = x;     // (gdb) ptype d3 → int&
    decltype((x))    d4 = x;     // (gdb) ptype d4 → int&（双括号！）
    decltype(x + 0)  d5 = 0;     // (gdb) ptype d5 → int

    auto r1 = add(1, 2.5);      // (gdb) ptype r1 → double
    auto r2 = add(3.14, 5);     // (gdb) ptype r2 → double

    cout << "所有 decltype 变量推导完成。请在 GDB 中用 ptype 验证。\n";
}

// -----------------------------------------------------------
// main
// -----------------------------------------------------------

int main()
{
    SetConsoleOutputCP(CP_UTF8);

    lesson1_decltype_basics();
    lesson2_trailing_return();
    lesson3_decltype_use_cases();
    lesson4_gdb_practice();

    cout << "\n========== 程序正常结束 ==========\n";
    return 0;
}

// ============================================================
// 今日要点总结
// ============================================================
//
// decltype 两条规则：
//   规则 A: decltype(变量名) → 变量的声明类型（保留 const 和引用）
//   规则 B: decltype(表达式) → 看值类别
//           - 左值(x)      → T&
//           - 将亡值(move)  → T&&
//           - 纯右值        → T
//
// 双括号陷阱：
//   decltype(x)  → int    （变量名，规则 A）
//   decltype((x))→ int&   （表达式是左值，规则 B）
//   只多了一对括号，类型不同！面试必考。
//
// 返回类型后置：
//   template<T,U> auto add(T a, U b) -> decltype(a+b)
//   参数列表之后才能用 a、b，所以返回类型写在箭头后面。
//
// decltype(auto)：
//   C++14 起可替代 auto -> decltype(...) 这种写法
//   保留引用和 const，auto 会丢引用
// ============================================================
