// ============================================================
// Week05 Day4：nullptr + using 别名 + 右值引用基础 — exercise.cpp
// ============================================================
// 编译：
//   g++ -std=c++17 -Wall -g -O0 exercise.cpp -o exercise.exe
//
// GDB 验证：
//   gdb ./exercise.exe
//   (gdb) break 需要验证的函数
//   (gdb) run
//   (gdb) ptype / next / step / bt
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
// 需求清单（请逐条完成，每条都要用 GDB 验证）
// ============================================================

// -----------------------------------------------------------
// 需求 1：nullptr vs NULL 的重载解析
// -----------------------------------------------------------
// ① 写两个重载函数：
//     void show(int x)        — 打印 "整数: x"
//     void show(void* p)      — 打印 "指针" 或 "空指针"
// ② 在 test_nullptr() 中：
//     - 调用 show(NULL) —— 观察调了哪个版本
//     - 调用 show(nullptr) —— 观察调了哪个版本
//     - 调用 show((void*)0) —— 观察调了哪个版本
// ③ 回答：为什么 NULL 和 nullptr 的行为不同？
//        如果你的同事在重载函数里用 NULL，可能会出什么 bug？

// TODO: 写两个 show() 重载
void show(int x)
{
    cout << "整数:" << x << endl;
}
void show(void* p)
{
    cout << "指针:" << p << endl;
}
// TODO: 写 test_nullptr() 函数
void test_nullptr()
{
    cout << "调用NULL(等价于0)\n";
    show(0);
    cout << "调用nullptr\n";
    show(nullptr);
    cout << "调用(void*)0\n";
    show((void*)0);
}

// -----------------------------------------------------------
// 需求 2：using 别名 + 模板别名
// -----------------------------------------------------------
// ① 用 using 给 unordered_map<string, int> 起个别名叫 ScoreMap
// ② 用 using 写模板别名 template<typename T> using Vec = ...;
// ③ 在 test_using() 中：
//     - 用 ScoreMap 创建变量并插入几组数据，打印
//     - 用 Vec<int> 创建变量（等价于 vector<int>），push 几个元素，打印
//     - 用 Vec<Vec<string>> 创建二维字符串数组（等价于 vector<vector<string>>）
//     - 填一组数据 ["A","B"] 和 ["C","D"]，打印
// ④ 回答：为什么 using 能写模板别名而 typedef 不行？

// TODO: 写 ScoreMap 别名
using ScoreMap = unordered_map<string,int>;
// TODO: 写 Vec 模板别名
template<typename T>
using Vec = vector<T>;
// TODO: 写 test_using() 函数
void test_using()
{
    ScoreMap a = {{"成功",11},{"cg",3},{"qfr",21}};
    a.insert({"插入数据",444});
    for(const auto& [key,value]:a)
    {
        cout << key << " " << value << endl;
    }
    cout << endl;
    Vec<int> b = {3,21,5};
    b.push_back(22);
    b.push_back(54);
    b.push_back(75);
    for(auto x:b)
    {
        cout << x << " ";
    }
    cout << endl;
    Vec<Vec<string>> c;
    c.push_back({"A","B"});
    c.push_back({"C","D"});
    for(auto s:c)
    {
        for(auto ss:s)
        {
            cout << ss <<" ";
        }
        cout << endl;
    }
    cout << endl;
}

// -----------------------------------------------------------
// 需求 3：左值右值判断
// -----------------------------------------------------------
// ① 写一个类 Widget，有构造函数和析构函数打印信息
// ② 在 test_lvalue_rvalue() 中判断以下表达式是左值还是右值：
//     - int x = 10;                    x
//     - 42
//     - Widget("tmp")                  临时构造的 Widget
//     - std::move(x)
//     - x + 42
//     - string("hello") + " world"
//     - int&& rr = 100;                rr
// ③ 用注释标注每个是左值还是右值，并写判断理由
//     （能 &取地址 → 左值；不能 → 右值）
// ④ 用 GDB 在 Widget 的构造和析构函数设断点，观察临时对象的生命周期

// TODO: 写 Widget 类
class Widget
{
private:
    string val_;
public:
    Widget(string val):val_(val)
    {
        cout << "构造Widget\n";
    }
    ~Widget()
    {
        cout << "析构Widget\n";
    }
};
// TODO: 写 test_lvalue_rvalue() 函数
void test_lvalue_rvalue()
{
      int x = 10;
      // x 是左值（有名字，能取地址 &x）

      // 42 是右值（字面量）

      // Widget("tmp") 是右值（临时对象）

      // std::move(x) 是右值（move转成将亡值）

      // x + 42 是右值（表达式结果是临时值）

      // string("hello") + " world" 是右值（operator+返回临时对象）

      int&& rr = 100;
      // rr 是左值（虽然类型是 int&&，但变量本身有名字有地址）
      // 这是最反直觉的一点：右值引用变量本身是左值
    
}

// -----------------------------------------------------------
// 需求 4：重载区分左值引用和右值引用
// -----------------------------------------------------------
// ① 写一个函数 process(string& s)   — 打印 "处理左值: s"
// ② 写 overload process(string&& s) — 打印 "处理右值: s"
// ③ 写 overload process(const string& s) — 打印 "处理只读: s"
// ④ 在 test_overload() 中：
//     - 创建 string s = "hello";
//     - 调用 process(s)            — 应该走 string&
//     - 调用 process(string("hi")) — 应该走 string&&
//     - 调用 process(move(s))      — 应该走 string&&
//     - 调用 process("literal")    — 观察走哪个版本
// ⑤ 用 GDB 在 process(string&&) 设断点，bt 看调用栈
// ⑥ 回答：如果只写 process(string& s) 而不写右值版本，
//        process(string("hi")) 能编译吗？为什么？

// TODO: 写三个 process() 重载
void process(string& s)
{
    cout << "处理左值:s\n";
}
void process(string&& s)
{
    cout << "处理右值:s\n";
}
void process(const string& s)
{
    cout << "处理只读:s\n";
}
// TODO: 写 test_overload() 函数
void test_overload()
{
    string a = "hello";
    process(a);
    process(string("hi"));
    process(move(a));
    process("literal");
}

// -----------------------------------------------------------
// 需求 5（综合）：用 GDB 跟踪右值引用
// -----------------------------------------------------------
// ① 写一个类 Buffer：
//     - 有 int* data_ 和 size_t size_ 两个成员
//     - 构造函数 Buffer(size_t n) 分配 n 个 int
//     - 析构函数释放 data_
//     - void fill(int val) 把所有元素填成 val
//     - void print() const 打印前 5 个元素
// ② 写一个函数 pass_by_rref(Buffer&& b)，接收右值引用
//    在里面调用 b.print()
// ③ 在 test_buffer() 中：
//     - 创建 Buffer tmp(10)，fill(7)
//     - 调用 pass_by_rref(move(tmp)) 转移给函数
//     - 用 GDB 在 pass_by_rref 设断点，print &b 查看地址
// ④ 思考：move(tmp) 之后 tmp 中的数据指针还有效吗？
//     （Buffer 没有写移动构造，所以数据还在）

// TODO: 写 Buffer 类

// TODO: 写 pass_by_rref() 函数

// TODO: 写 test_buffer() 函数


// -----------------------------------------------------------
// main
// -----------------------------------------------------------

int main()
{
    SetConsoleOutputCP(CP_UTF8);

    cout << "===== Week05 Day4 nullptr + using + 右值引用 练习 =====\n";

    // TODO: 写完每个 test 函数后取消注释对应的调用
    test_nullptr();
    test_using();
    test_lvalue_rvalue();
    test_overload();
    // test_buffer();

    cout << "\n===== 练习完成 =====\n";
    cout << "别忘了用 GDB 验证每个需求！\n";
    return 0;
}
