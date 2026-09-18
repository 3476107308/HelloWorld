// ============================================================
// Week05 Day2：auto 类型推导 — exercise.cpp（独立练习）
// ============================================================
// 编译：
//   g++ -std=c++17 -Wall -g -O0 exercise.cpp -o exercise.exe
//
// GDB 验证：
//   gdb ./exercise.exe
//   (gdb) break 需要验证的函数
//   (gdb) run
//   (gdb) ptype 变量名       ← 查看 auto 推导出的实际类型
// ============================================================

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <windows.h>

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::map;

// ============================================================
// 需求清单（请逐条完成，每条都要用 GDB ptype 验证类型）
// ============================================================

// -----------------------------------------------------------
// 需求 1：理解 auto 为什么丢 const
// -----------------------------------------------------------
// ① 写一个函数 get_message()，返回 const string（内容是 "Hello"）
// ② 在 test_auto_strips_const() 中：
//     - 用 auto a = get_message(); 接收
//     - 用 auto& b = get_message(); 接收
//     - 用 const auto& c = get_message(); 接收
// ③ 用 GDB ptype 查看 a、b、c 的实际类型
// ④ 尝试修改 a（如果 a 是 string 不是 const，就可以改）
//    尝试修改 b（应该编译不通过——注释掉并说明原因）
// ⑤ 回答：如果只读接收，用 auto 有什么隐患？
// 隐患是auto会创建副本 导致额外开销 
// TODO: 写 get_message() 函数
const string get_message()
{
    return "Hello";
}
// TODO: 写 test_auto_strips_const() 函数
void test_auto_strips_const()
{
    auto a = get_message();
    auto& b = get_message();
    const auto& c = get_message();
}
// -----------------------------------------------------------
// 需求 2：范围 for 中 auto / auto& / const auto& 的区别
// -----------------------------------------------------------
// ① 创建一个 vector<string> words = {"C++", "Python", "Rust", "Go"};
// ② 写三个版本的范围 for 循环：
//     - for (auto w : words)    { w += "语言"; }  // 版本 A
//     - for (auto& w : words)   { w += "语言"; }  // 版本 B
//     - for (const auto& w : words) { /* 只打印 */ }  // 版本 C
// ③ 在版本 A 之后打印 words，看看内容变了没有
//    在版本 B 之后打印 words，看看内容变了没有
// ④ 用 GDB 在循环体中设断点，ptype w 查看三个版本 w 的类型
// ⑤ 回答：如果 vector 里存的是大对象（如 string），用 auto 遍历会有什么性能问题？

// TODO: 写 test_range_for() 函数
void test_range_for()
{
    vector<string> words = {"C++","Python","Rust","Go"};
    for(auto x:words)
    {
        x += "语言";
    }

    for(auto x:words)
    cout << x << " " ;
    cout << endl;

    for(auto& x:words)
    x += "语言";

    for(auto x:words)
    cout << x << " " ;
    cout << endl;

    for(const auto& x:words)
    cout << x <<" ";
    cout << endl;
}
// -----------------------------------------------------------
// 需求 3：auto 与迭代器
// -----------------------------------------------------------
// ① 创建一个 map<string, int> scores = {{"张三",85}, {"李四",92}, {"王五",78}};
// ② 用 auto 写迭代器循环，打印所有键值对
//    提示：for (auto it = scores.begin(); ...)
// ③ 用 GDB ptype 查看 it 的实际类型
// ④ 用 范围 for + auto（结构化绑定 C++17）改写：
//     for (auto& [name, score] : scores) { ... }
// ⑤ 回答：使用 auto 写迭代器循环，代码量减少了多少？（对比显式写完整类型名）

// TODO: 写 test_iterator() 函数
void test_iterator()
{
    map<string,int> scores = {{"张三",85},{"李四",92},{"王五",78}};
    for(auto it = scores.begin();it != scores.end();it++)
    {
        cout << it->first << " " << it->second << endl;
    }
    for(const auto& [name,score]:scores)
    {
        cout << name << " " << score << endl; 
    }
}
// -----------------------------------------------------------
// 需求 4：auto 推导 initializer_list
// -----------------------------------------------------------
// ① 写 auto x = {1, 2, 3, 4, 5};
// ② 用 GDB ptype 查看 x 的类型（应该是 initializer_list<int>，不是 int）
// ③ 打印 x.size()
// ④ 用 {} 初始化一个 auto y = 42;（正确，y 是 int）
//    和 auto z = {42};（z 是 initializer_list<int>）
//    用 ptype 分别查看 y 和 z 的类型
// ⑤ 回答：什么时候 auto 会推导出 initializer_list？为什么这个陷阱容易出错？

// TODO: 写 test_initializer_list() 函数
void test_initializer_list()
{
    auto x = {1,2,3,4,5};
    cout << x.size() << endl;
    auto y = 42;
    auto z = {42};

}
// -----------------------------------------------------------
// 需求 5：用 GDB 跟踪 auto 变量的类型推导
// -----------------------------------------------------------
// ① 把需求 1~4 的代码全部写完
// ② 在 main 中依次调用四个 test 函数
// ③ 用 GDB 在 test_auto_strips_const 中设断点：
//     (gdb) break test_auto_strips_const
//     (gdb) run
//     (gdb) next  ← 每走一步用 ptype 确认 auto 变量类型
// ④ 记录至少 5 个 ptype 的验证结果

// -----------------------------------------------------------
// main
// -----------------------------------------------------------

int main()
{
    SetConsoleOutputCP(CP_UTF8);

    cout << "===== Week05 Day2 auto 练习 =====\n";

    // TODO: 写完每个 test 函数后取消注释对应的调用
    test_auto_strips_const();
    test_range_for();
    test_iterator();
    test_initializer_list();

    cout << "\n===== 练习完成 =====\n";
    cout << "别忘了用 GDB ptype 验证每个 auto 变量的类型！\n";
    return 0;
}
