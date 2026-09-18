// ============================================================
// Week05 Day2：auto 类型推导 — example.cpp（教学示例）
// ============================================================
// 编译：
//   g++ -std=c++17 -Wall -g -O0 example.cpp -o example.exe
// 调试：
//   gdb ./example.exe
//
// 补充视频：大丙 C++11 新特性详解「第二部分：auto 类型推导」
//   https://space.bilibili.com/147020887/lists/6687062
// ============================================================

#include <iostream>
#include <vector>
#include <string>
#include <typeinfo>   // typeid
#include <type_traits> // is_same 编译期类型判断
#include <windows.h>

using std::cout;
using std::endl;
using std::vector;
using std::string;

// ============================================================
// 第 1 节：auto 的核心推导规则
// ============================================================
// auto 的规则很简单：它模仿"模板参数推导"。
// 关键记住一句话：auto 会丢掉 const 和引用（除非你显式要求保留）。
//
// auto       → 值语义，丢掉 const、丢掉引用
// auto&      → 引用语义，保留底层 const
// const auto&→ 常量引用，可以绑定到临时对象
// auto&&     → 万能引用（转发引用），左值→左值引用，右值→右值引用
// ============================================================

// 一个返回 const 引用的辅助函数——用来测试接收端的推导
const vector<int>& get_data()
{
    static vector<int> v = {1, 2, 3, 4, 5};
    return v;
}

// 一个返回临时对象的辅助函数
vector<int> make_data()
{
    return {10, 20, 30};
}

void lesson1_auto_basic_rules()
{
    cout << "\n========== 第 1 节：auto 核心推导规则 ==========\n";

    // -------------------------------------------------------
    // 规则 1：auto 会丢掉 const 和引用（值语义）
    // -------------------------------------------------------
    {
        const int x = 42;
        auto a = x;          // a 的类型是 int（丢了 const）
        // a = 100;          // 可以修改！因为 a 是 int，不是 const int

        const int& ref = x;  // ref 是 const int&
        auto b = ref;        // b 的类型是 int（丢了 const 和引用！）
        // b = 200;          // 可以修改

        cout << "规则 1: const int x=42; auto a=x;       → a 是 "
             << (std::is_same<decltype(a), int>::value ? "int" : "???") << endl;
        cout << "规则 1: const int& ref=x; auto b=ref;   → b 是 "
             << (std::is_same<decltype(b), int>::value ? "int" : "???") << endl;
        // 如何验证？GDB 中：
        //   (gdb) ptype a   → 输出 int
        //   (gdb) ptype b   → 输出 int（注意：不是 const int，也不是 int&）
    }

    // -------------------------------------------------------
    // 规则 2：auto& 保留底层 const，不能绑定到临时对象
    // -------------------------------------------------------
    {
        const int x = 42;
        auto& c = x;         // c 的类型是 const int&（保留了 const）
        // c = 100;          // 编译错误！c 是 const int&，不能修改

        // auto& d = 100;    // 编译错误！不能把右值（字面量）绑定到 auto&

        cout << "规则 2: const int x=42; auto& c=x;     → c 是 const int&\n";
    }

    // -------------------------------------------------------
    // 规则 3：const auto& 可以绑定到任何东西（万能"只读"接收器）
    // -------------------------------------------------------
    {
        const auto& d = 100;       // OK：可以绑定临时对象（生命周期延长）
        const auto& e = get_data(); // OK：接收函数返回的 const 引用

        cout << "规则 3: const auto& d=100;              → d=100，类型 const int&\n";
        cout << "规则 3: const auto& e=get_data();       → e 是 const vector<int>&\n";
    }

    // -------------------------------------------------------
    // 规则 4：auto&& 是万能引用（转发引用）
    // -------------------------------------------------------
    {
        int x = 10;
        auto&& f = x;        // x 是左值 → f 推导为 int&（左值引用）
        auto&& g = 100;      // 100 是右值 → g 推导为 int&&（右值引用）
        auto&& h = get_data(); // get_data() 返回 const int& → h 推导为 const int&

        cout << "规则 4: int x=10; auto&& f=x;          → f 是 int&\n";
        cout << "规则 4: auto&& g=100;                   → g 是 int&&\n";
        cout << "规则 4: auto&& h=get_data();            → h 是 const vector<int>&\n";
    }

    // -------------------------------------------------------
    // 规则 5：用函数返回值来练习——检验你是否理解
    // -------------------------------------------------------
    {
        /*
         * get_data() 返回 const vector<int>&
         * 思考：下面四行分别推导出什么类型？
         */

        auto        v1 = get_data();  // vector<int>（拷贝了！丢了 const 和引用）
        auto&       v2 = get_data();  // const vector<int>&（保留了 const）
        const auto& v3 = get_data();  // const vector<int>&
        auto&&      v4 = get_data();  // const vector<int>&（右值引用折叠为左值引用）

        // 验证：用 typeid 看类型（GDB 中更清楚：ptype v1）
        // typeid 会丢掉引用和顶层 const，所以下面不是最好的验证方式
        // 更好的方式是在 GDB 中用 ptype 命令
        cout << "\n验证函数返回值的接收:\n";
        cout << "v1 (auto)        : 是拷贝, &v1=" << &v1 << ", &原始=" << &get_data() << endl;
        cout << "v2 (auto&)       : 是引用, &v2=" << &v2 << ", &原始=" << &get_data() << "（地址相同！）\n";
        // ★ v2 地址和 get_data() 内部 static v 的地址相同，说明 v2 没拷贝，直接引用
    }

    // -------------------------------------------------------
    // GDB 练习提示
    // -------------------------------------------------------
    // 在下面这行设断点（break 行号），用 ptype 验证每个变量的类型：
    //   (gdb) ptype v1    → std::vector<int>
    //   (gdb) ptype v2    → const std::vector<int> &
    //   (gdb) ptype v3    → const std::vector<int> &
    //   (gdb) ptype v4    → const std::vector<int> &
    //   (gdb) print &v1
    //   (gdb) print &get_data()   ← 两个地址不一样！v1 是拷贝
    //   (gdb) print &v2
    //   (gdb) print &get_data()   ← 两个地址一样！v2 是引用
}

// ============================================================
// 第 2 节：auto 的实际应用场景
// ============================================================

void lesson2_auto_in_practice()
{
    cout << "\n========== 第 2 节：auto 的实际应用 ==========\n";

    // -------------------------------------------------------
    // 场景 1：迭代器——不用写一长串类型名
    // -------------------------------------------------------
    {
        vector<string> names = {"Alice", "Bob", "Charlie", "Diana"};

        // 不用 auto：
        // for (vector<string>::iterator it = names.begin(); it != names.end(); ++it)
        //     cout << *it << endl;

        // 用 auto：
        for (auto it = names.begin(); it != names.end(); ++it)
        {
            cout << *it << " ";
        }
        cout << endl;
        // GDB: ptype it → __gnu_cxx::__normal_iterator<std::string*, ...>
        // 不用 auto 的话光这个类型名就得写三行
    }

    // -------------------------------------------------------
    // 场景 2：范围 for 循环——最推荐
    // -------------------------------------------------------
    {
        vector<int> nums = {1, 2, 3, 4, 5};

        // auto x : 拷贝每个元素（不修改原数据）
        cout << "auto x (拷贝): ";
        for (auto x : nums)
        {
            x *= 2;  // 只改了拷贝，nums 不变
        }
        for (auto x : nums) cout << x << " ";
        cout << "  ← 原数据没变" << endl;

        // auto& x : 引用每个元素（可以修改原数据）
        cout << "auto& x (引用): ";
        for (auto& x : nums)
        {
            x *= 2;  // 直接修改 nums 中的元素
        }
        for (auto x : nums) cout << x << " ";
        cout << "  ← 原数据变了！" << endl;

        // const auto& x : 只读引用（避免拷贝，但不能修改）
        cout << "const auto& x (只读): ";
        for (const auto& x : nums)
        {
            // x *= 2;  // 编译错误！const 不能修改
            cout << x << " ";
        }
        cout << endl;
    }

    // -------------------------------------------------------
    // 场景 3：lambda 表达式
    // -------------------------------------------------------
    {
        // lambda 的类型是编译器生成的匿名类型，你写不出来
        // 只能用 auto 接收
        auto is_even = [](int n) { return n % 2 == 0; };

        cout << "2 是偶数? " << (is_even(2) ? "是" : "否") << endl;
        cout << "3 是偶数? " << (is_even(3) ? "是" : "否") << endl;

        // 没有 auto 的话，只能用 std::function（有额外开销）：
        // std::function<bool(int)> is_even = [](int n) { return n % 2 == 0; };
        // auto 方式零开销——编译器直接生成类型，没有间接调用
    }

    // -------------------------------------------------------
    // 场景 4：模板函数的返回值
    // -------------------------------------------------------
    {
        auto add = [](auto a, auto b) {  // C++14 泛型 lambda
            return a + b;
        };

        cout << "add(1, 2)       = " << add(1, 2) << endl;
        cout << "add(1.5, 2.7)   = " << add(1.5, 2.7) << endl;
        cout << "add(1, 2.5)     = " << add(1, 2.5) << "（int + double → double）" << endl;
    }
}

// ============================================================
// 第 3 节：auto 的陷阱
// ============================================================

void lesson3_auto_pitfalls()
{
    cout << "\n========== 第 3 节：auto 的陷阱 ==========\n";

    // -------------------------------------------------------
    // 陷阱 1：auto 推导 initializer_list
    // -------------------------------------------------------
    {
        auto x = {1, 2, 3};     // x 的类型是 std::initializer_list<int>！不是 int！

        cout << "auto x = {1,2,3}; → x 的类型是 initializer_list<int>\n";
        cout << "x.size() = " << x.size() << endl;
        // 如果期望 x 是 int，应该写 auto x = 1;
        // auto 遇到花括号初始化列表{} 会推导成 initializer_list（C++14 有限制）
    }

    // -------------------------------------------------------
    // 陷阱 2：auto 推导 vector<bool> 的元素
    // -------------------------------------------------------
    {
        vector<bool> flags = {true, false, true};
        auto f = flags[0];      // f 的类型是 std::vector<bool>::reference
                                 // 不是 bool！
        // vector<bool> 比较特殊，它的 operator[] 返回的是"代理引用"，不是 bool&
        // 这里一般用 bool f = flags[0]; 或直接写类型
        cout << "vector<bool> flags[0] 用 auto 接收 → 不是 bool，是代理引用类型\n";
        cout << "建议：涉及 vector<bool> 时显式写 bool，不要用 auto\n";
    }

    // -------------------------------------------------------
    // 陷阱 3：auto 丢引用导致意外拷贝
    // -------------------------------------------------------
    {
        vector<string> names = {"张三", "李四", "王五"};
        for (auto s : names)   // auto 是 string，每次都拷贝了整个 string
        {
            s += "同学";       // 改的是拷贝，names 不变
        }
        cout << "auto s 遍历后: ";
        for (auto& s : names) cout << s << " ";  // 还是原样
        cout << "← 没变！（因为 auto 是拷贝）" << endl;

        // 正确做法：想修改用 auto&，只看不改用 const auto&
        for (auto& s : names) s += "同学";
        cout << "auto& s 遍历后: ";
        for (auto& s : names) cout << s << " ";
        cout << endl;
    }

    // -------------------------------------------------------
    // 陷阱 4：看代码猜类型的能力
    // -------------------------------------------------------
    {
        const vector<int> cv = {0, 1, 2};
        auto a1 = cv;          // vector<int>（拷贝，丢了 const）
        auto& a2 = cv;         // const vector<int>&
        auto a3 = cv[0];       // int（下标返回 const int&，auto 丢掉 const 和引用）
        decltype(auto) a4 = cv[0]; // const int&（decltype(auto) 完美保留——Day 3 会学）

        cout << "\n看代码猜类型:\n";
        cout << "const vector<int> cv; auto a1 = cv;      → vector<int>\n";
        cout << "const vector<int> cv; auto& a2 = cv;     → const vector<int>&\n";
        cout << "const vector<int> cv; auto a3 = cv[0];   → int\n";
        cout << "const vector<int> cv; decltype(auto) a4  → const int&\n";
    }
}

// ============================================================
// 第 4 节：用 GDB 的 ptype 验证 auto 推导
// ============================================================
// 在下面函数设断点，用 ptype 验证每个 auto 变量的类型
// ============================================================

void lesson4_gdb_practice()
{
    cout << "\n========== 第 4 节：GDB ptype 练习 ==========\n";

    // ---- 请用 GDB 逐行验证以下变量的类型 ----
    // (gdb) break lesson4_gdb_practice
    // (gdb) run
    // (gdb) next  ← 每执行一句就 ptype 一下

    int raw = 42;                    // (gdb) ptype raw   → int
    const int c_raw = 100;           // (gdb) ptype c_raw → const int

    auto a1 = raw;                   // (gdb) ptype a1    → int
    auto a2 = c_raw;                 // (gdb) ptype a2    → int（丢了 const！）
    const auto a3 = c_raw;           // (gdb) ptype a3    → const int

    auto& a4 = raw;                  // (gdb) ptype a4    → int&
    auto& a5 = c_raw;                // (gdb) ptype a5    → const int&
    const auto& a6 = raw;            // (gdb) ptype a6    → const int&

    auto&& a7 = 100;                 // (gdb) ptype a7    → int&&（右值引用）
    auto&& a8 = raw;                 // (gdb) ptype a8    → int&（万能引用！）
    auto&& a9 = std::move(raw);      // (gdb) ptype a9    → int&&

    vector<int> v = {0, 1, 2};
    auto a10 = v[0];                 // (gdb) ptype a10   → int（丢了引用）
    auto& a11 = v[0];                // (gdb) ptype a11   → int&
    decltype(auto) a12 = v[0];       // (gdb) ptype a12   → int&（decltype(auto) 保留引用）

    cout << "所有 auto 变量推导完成。请在 GDB 中用 ptype 逐一验证。\n";
    // 提示：这里的变量在优化下可能被优化掉，确保用 -O0 编译
}

// -----------------------------------------------------------
// main
// -----------------------------------------------------------

int main()
{
    SetConsoleOutputCP(CP_UTF8);

    lesson1_auto_basic_rules();
    lesson2_auto_in_practice();
    lesson3_auto_pitfalls();
    lesson4_gdb_practice();

    cout << "\n========== 程序正常结束 ==========\n";
    return 0;
}

// ============================================================
// 今日要点总结
// ============================================================
//
// ┌──────────────────────────────────────────────────────────┐
// │ 声明               │ 推导结果              │ 何时用       │
// ├──────────────────────────────────────────────────────────┤
// │ auto x = expr      │ 值类型（丢const/引用） │ 需要拷贝    │
// │ auto& x = expr     │ 引用（保留const）     │ 需要修改    │
// │ const auto& x = e  │ const引用（可绑临时） │ 只读，避免拷贝│
// │ auto&& x = expr    │ 万能引用              │ 模板/转发   │
// └──────────────────────────────────────────────────────────┘
//
// 核心原则：
//   - 想修改 → auto&
//   - 只想看 → const auto&
//   - 要拷贝 → auto
//   - 模板/转发 → auto&&
// ============================================================
