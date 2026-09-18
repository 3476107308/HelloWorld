// ============================================================
// Week05 Day3：decltype + 返回类型后置 — exercise.cpp（独立练习）
// ============================================================
// 编译：
//   g++ -std=c++17 -Wall -g -O0 exercise.cpp -o exercise.exe
//
// GDB 验证：
//   gdb ./exercise.exe
//   (gdb) break 需要验证的函数
//   (gdb) run
//   (gdb) ptype 变量名       ← 查看 decltype 推导出的实际类型
// ============================================================

#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <type_traits>
#include <windows.h>

using std::cout;
using std::endl;
using std::vector;
using std::string;

// ============================================================
// 需求清单（请逐条完成，每条都要用 GDB ptype 验证类型）
// ============================================================

// -----------------------------------------------------------
// 需求 1：decltype(变量) vs decltype((变量))
// -----------------------------------------------------------
// ① 在 test_decltype_paren() 中：
//     - 声明 int a = 42;
//     - 用 decltype(a) 声明变量 b，用 GDB ptype 验证 b 是 int
//     - 用 decltype((a)) 声明变量 c，用 GDB ptype 验证 c 是 int&
//     - 修改 c 的值，打印 a，看 a 是否也跟着变了
// ② 声明 const int ca = 100;
//     - 用 decltype(ca) 声明 d，用 GDB ptype 验证 d 是 const int
//     - 用 decltype((ca)) 声明 e，用 GDB ptype 验证 e 是 const int&
// ③ 回答：为什么 decltype((a)) 是引用，而 decltype(a) 不是？

// TODO: 写 test_decltype_paren() 函数
void test_decltype_paren()
{
    int a = 42;
    decltype(a) b = 41;
    decltype((a)) c = a;
    c = 91;
    const int ca = 100;
    decltype(ca) d = ca;
    decltype((ca)) e = ca;
}

// -----------------------------------------------------------
// 需求 2：返回类型后置 —— 模板函数
// -----------------------------------------------------------
// ① 写模板函数 multiply(T a, U b)，用 auto + -> decltype(a*b) 作为返回类型后置
//    让它能正确返回 int*double → double、double*double → double 等
// ② 在 test_trailing_return() 中调用 multiply 三次：
//     - multiply(3, 4)      → 期望返回 int
//     - multiply(3, 4.5)    → 期望返回 double
//     - multiply(2.5, 3.0)  → 期望返回 double
//    用 cout 打印结果，用 GDB ptype 验证每次返回的类型
// ③ 写一个不带返回类型后置的版本 multiply_bad，返回类型写死为 int
//    对比 multiply_bad(3, 4.5) 的返回值（会被截断）
// ④ 回答：什么时候必须用返回类型后置？为什么把 decltype 放参数前面不行？

// TODO: 写 multiply() 模板函数
template<typename T,typename U>
auto multiply(T a,U b)->decltype(a*b)
{
    return a*b;
}

// TODO: 写 test_trailing_return() 函数
void test_trailing_return()
{
    cout << multiply(3,4) << endl;
    cout << multiply(3,4.5) << endl;
    cout << multiply(2.5,3.0) << endl;
}

// -----------------------------------------------------------
// 需求 3：decltype 声明迭代器
// -----------------------------------------------------------
// ① 创建一个 vector<double> v = {1.1, 2.2, 3.3, 4.4, 5.5};
// ② 用 decltype(v)::iterator 声明迭代器 it（而不是用 auto 或手写类型名）
// ③ 用这个迭代器遍历并打印所有元素
// ④ 用 GDB ptype 验证 decltype(v)::iterator 推导出的类型
//    对比：直接用 auto it = v.begin() 哪个写法更好？
// ⑤ 给 v 加一个元素：用 decltype(v[0]) 声明变量类型，存 v[0] 的值

// TODO: 写 test_decltype_iterator() 函数
void test_decltype_iterator()
{
    vector<double> v = {1.1,2.2,3.3,4.4,5.5};
    decltype(v)::iterator it = v.begin();
    for(;it != v.end();it++)
    {
        cout << *it << " ";
    }
    cout << endl;
    decltype(v[0]) v5 = v[0];
    v.push_back(v5);
}

// -----------------------------------------------------------
// 需求 4：decltype + std::declval 推导表达式类型
// -----------------------------------------------------------
// ① 有一个类 NoDefault，它没有默认构造函数（只有带参构造）
//    写出来这个类，带一个 int 参数的构造函数
// ② 用 decltype + std::declval 推导 NoDefault + NoDefault 的类型
//    （假设有一个 operator+，返回 NoDefault）
//    提示：decltype(std::declval<NoDefault>() + std::declval<NoDefault>())
// ③ 用 using 给这个类型起别名 SumType
// ④ 回答：std::declval 的作用是什么？为什么不能直接 NoDefault{} + NoDefault{}？

// TODO: 写 NoDefault 类
class NoDefault
{
private:
    int x_;
public:
    NoDefault(int x):x_(x){}
    int x()const{return x_;}
    friend NoDefault operator+(const NoDefault& a,const NoDefault& b);
};
// TODO: 写 operator+(const NoDefault&, const NoDefault&)
NoDefault operator+(const NoDefault& a,const NoDefault& b)
{
    return NoDefault(a.x()+b.x());
}
// TODO: 写 test_declval() 函数
void test_declval()
{
    using SumType = decltype(std::declval<NoDefault>() + std::declval<NoDefault>());

    cout << "SumType 就是 NoDefault: "
         << (std::is_same<SumType, NoDefault>::value ? "是 ✓" : "不是 ✗") << endl;

    NoDefault a(10), b(20);
    NoDefault c = a + b;
    cout << "10 + 20 = " << c.x() << endl;
}

// -----------------------------------------------------------
// 需求 5（综合）：用 GDB 验证所有 decltype 推导
// -----------------------------------------------------------
// ① 把需求 1~4 的代码全部写完
// ② 在 main 中依次调用四个 test 函数
// ③ 用 GDB 在每个 test 函数中设断点：
//     (gdb) break test_decltype_paren
//     (gdb) run
//     (gdb) next ← 每走一步用 ptype 确认 decltype 变量的类型
// ④ 记录至少 4 个 ptype 的验证结果到 review.md
// ⑤ 特别验证：需求 2 中 multiply 不同参数时返回类型的区别

// -----------------------------------------------------------
// main
// -----------------------------------------------------------

int main()
{
    SetConsoleOutputCP(CP_UTF8);

    cout << "===== Week05 Day3 decltype 练习 =====\n";

    // TODO: 写完每个 test 函数后取消注释对应的调用
    test_decltype_paren();
    test_trailing_return();
    test_decltype_iterator();
    test_declval();

    cout << "\n===== 练习完成 =====\n";
    cout << "别忘了用 GDB ptype 验证每个 decltype 变量的类型！\n";
    return 0;
}
