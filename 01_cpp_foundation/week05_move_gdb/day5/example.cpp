// ============================================================
// Week05 Day5：std::move + std::forward 完美转发 — example.cpp（教学示例）
// ============================================================
// 编译：
//   g++ -std=c++17 -Wall -g -O0 example.cpp -o example.exe
// 调试：
//   gdb ./example.exe
//
// 补充视频：大丙 C++11 新特性详解「第三部分：std::move、std::forward、完美转发」
//   https://space.bilibili.com/147020887/lists/6687062
// ============================================================

#include <iostream>
#include <vector>
#include <string>
#include <utility>      // std::move, std::forward
#include <windows.h>

using std::cout;
using std::endl;
using std::vector;
using std::string;

// ============================================================
// 第 1 节：std::move 的本质 —— 只是一个 cast
// ============================================================
// std::move 不移动任何数据！它的实现等价于：
//   template<typename T>
//   decltype(auto) move(T&& t) noexcept {
//       return static_cast<remove_reference_t<T>&&>(t);
//   }
// 就是把传入的东西无条件转成右值引用，仅此而已。
// 真正的"移动"发生在移动构造函数/移动赋值运算符里。
// ============================================================

struct MoveDemo
{
    string data_;

    MoveDemo(const string& s) : data_(s)
    {
        cout << "[构造] data_ = \"" << data_ << "\"\n";
    }

    // 拷贝构造 —— 深拷贝，开销大
    MoveDemo(const MoveDemo& other) : data_(other.data_)
    {
        cout << "[拷贝构造] 深拷贝 \"" << data_ << "\"\n";
    }

    // 移动构造 —— 偷走资源，开销小
    // 关键：参数是 MoveDemo&&（右值引用），标记 noexcept
    MoveDemo(MoveDemo&& other) noexcept : data_(std::move(other.data_))
    {
        cout << "[移动构造] 偷走 \"" << data_ << "\"\n";
        // other.data_ 现在是空字符串（string 的移动构造会清空源对象）
    }

    ~MoveDemo()
    {
        cout << "[析构] data_ = \"" << data_ << "\"\n";
    }
};

void lesson1_move_is_just_cast()
{
    cout << "\n========== 第 1 节：std::move 只是 cast ==========\n";

    // -------------------------------------------------------
    // 实验 1：move 本身不移动
    // -------------------------------------------------------
    {
        cout << "\n--- 实验1：move 本身不移动 ---\n";
        MoveDemo a("hello");

        // std::move(a) 只是把 a 转成 MoveDemo&&（右值引用）
        // 这一行不会调用任何构造函数，不会有任何数据转移
        // std::move(a);          // 光写这一行什么都不发生

        // 真正的移动发生在这里：用 move(a) 的结果初始化 b
        // b 从 a 的右值引用构造 → 调用移动构造函数
        MoveDemo b(std::move(a));   // 调用 MoveDemo(MoveDemo&&)

        cout << "a.data_ = \"" << a.data_ << "\"（源对象被掏空）\n";
        cout << "b.data_ = \"" << b.data_ << "\"（资源已转移给 b）\n";
    }

    // -------------------------------------------------------
    // 实验 2：没有移动构造函数时，move 退化为拷贝
    // -------------------------------------------------------
    {
        cout << "\n--- 实验2：没有移动构造，move 退化为拷贝 ---\n";
        struct NoMove
        {
            int x;
            NoMove(int v) : x(v) {}
            NoMove(const NoMove& o) : x(o.x)
            {
                cout << "  [拷贝构造] NoMove\n";
            }
            // 没有移动构造！右值引用能绑定，但会选拷贝构造（const T& 能绑右值）
        };

        NoMove nm1(42);
        NoMove nm2(std::move(nm1));   // 期望移动，实际走拷贝！
        cout << "nm1.x = " << nm1.x << endl;  // 42，数据还在，没被搬走
    }

    // -------------------------------------------------------
    // 实验 3：move 之后源对象处于什么状态
    // -------------------------------------------------------
    {
        cout << "\n--- 实验3：move 后源对象状态 ---\n";
        // C++ 标准：move 后的对象处于"有效但未指定"状态
        // - 可以析构（必须能安全析构）
        // - 可以赋值（重新给它新值）
        // - 不能假设它的值（可能是空的、旧的、随机的）
        //
        // 实践中的惯例：
        // - 标准库容器/string：move 后源对象变空
        // - 原始指针/内置类型：move 后值不变（原始类型没有移动构造）

        string s = "hello";
        string t = std::move(s);
        cout << "s = \"" << s << "\"（空，string 的移动构造会清空源）\n";
        cout << "t = \"" << t << "\"\n";

        int x = 100;
        int y = std::move(x);   // int 没有移动构造，就是普通拷贝
        cout << "x = " << x << ", y = " << y << "（int 的 move 就是拷贝）\n";
    }
}

// ============================================================
// 第 2 节：万能引用（转发引用）T&&
// ============================================================
// 规则：当 T 是模板参数且 T 需要推导时，T&& 是万能引用
//   - 传入左值 → T 推导为 X&  → 引用折叠：X& + && = X&
//   - 传入右值 → T 推导为 X   → X&& 就是右值引用
//
// 不是万能引用的 T&&：
//   - 具体类型 + &&（如 string&&）→ 纯粹的右值引用
//   - 不需要推导的 T&&（如 vector<int>&&）→ 纯粹的右值引用
// ============================================================

// 这是具体类型 + &&，是纯粹的右值引用，不能绑左值
void eat_rvalue(string&& s)
{
    cout << "  吃右值: " << s << endl;
}

// 这是万能引用——模板参数 T 需要推导
template<typename T>
void universal_ref_demo(T&& arg)
{
    // arg 的类型是什么？
    // 传入左值 → T = X&  → T&& = X&  → arg 是 X&（左值引用）
    // 传入右值 → T = X   → T&& = X&& → arg 是 X&&（右值引用）
    // 但记住：arg 本身永远是左值！
    cout << "  arg 本身是左值（有名字）\n";
}

void lesson2_universal_reference()
{
    cout << "\n========== 第 2 节：万能引用 T&& ==========\n";

    string s = "hello";

    // eat_rvalue(s);          // 编译错误！s 是左值，string&& 不接受左值
    eat_rvalue(std::move(s));  // OK，move 转成右值
    eat_rvalue(string("hi"));  // OK，临时对象是右值

    cout << "万能引用——传左值:";
    universal_ref_demo(s);          // T 推导为 string&，T&& = string&

    cout << "万能引用——传右值:";
    universal_ref_demo(string("x"));// T 推导为 string，T&& = string&&

    // 怎么区分？
    // 口诀：T 需要推导（模板/auto）+ && → 万能引用
    //      具体类型 + && → 纯右值引用
}

// ============================================================
// 第 3 节：std::forward —— 完美转发
// ============================================================
// 问题：万能引用拿到参数后，参数本身是左值，直接传递会丢失原始值类别。
//   template<typename T>
//   void wrapper(T&& arg) {
//       use(arg);   // arg 是左值 → 永远调左值版本！不对！
//   }
//
// 解决：std::forward<T>(arg)
//   - 如果 T 是 X&（传入的是左值） → forward 返回 X&（左值引用）
//   - 如果 T 是 X （传入的是右值） → forward 返回 X&&（右值引用）
//   - 效果：保持原始参数的值类别不变
// ============================================================

// 三个重载，用来观察转发结果
void use(int& x)       { cout << "  → use(int&) 左值, x = " << x << endl; }
void use(const int& x) { cout << "  → use(const int&) 只读, x = " << x << endl; }
void use(int&& x)      { cout << "  → use(int&&) 右值, x = " << x << endl; }

// 错误示范：不用 forward，直接传
template<typename T>
void bad_wrapper(T&& arg)
{
    // arg 本身是左值（有名字） → 永远走 use(int&) 版本！
    use(arg);
}

// 正确示范：用 forward 保持原始值类别
template<typename T>
void good_wrapper(T&& arg)
{
    // forward<T> 根据 T 判断：
    //   T = int&  → forward 返回 int&  → use 收到左值
    //   T = int   → forward 返回 int&& → use 收到右值
    use(std::forward<T>(arg));
}

void lesson3_perfect_forwarding()
{
    cout << "\n========== 第 3 节：std::forward 完美转发 ==========\n";

    int x = 42;
    const int cx = 100;

    cout << "\n--- bad_wrapper（不用 forward）---\n";
    cout << "传入左值 x:";
    bad_wrapper(x);              // 期望左值 ✓（碰巧对了）
    cout << "传入右值 42:";
    bad_wrapper(42);             // 期望右值 ✗ → 走了左值版本！

    cout << "\n--- good_wrapper（用 forward）---\n";
    cout << "传入左值 x:";
    good_wrapper(x);             // 左值 → use(int&)
    cout << "传入右值 42:";
    good_wrapper(42);            // 右值 → use(int&&)
    cout << "传入 const 左值 cx:";
    good_wrapper(cx);            // const左值 → use(const int&)

    // forward 和 move 的区别：
    //   move(arg)       = 无条件转成右值
    //   forward<T>(arg) = 有条件转发（左值→左值，右值→右值）
}

// ============================================================
// 第 4 节：移动构造函数和移动赋值 —— 手写
// ============================================================
// 移动构造：从右值偷资源，源对象置空
// 移动赋值：先释放自己的资源，再从右值偷资源
//
// 关键点：
//   1. 参数是 T&&（非 const 右值引用），因为要修改源对象（清空它）
//   2. 标记 noexcept —— 标准库容器在 resize 时只有 move 是 noexcept 的
//      才会用移动，否则会退化到拷贝
//   3. 都偷走后，让源对象处于安全析构状态（如设 nullptr）
// ============================================================

class Buffer
{
private:
    int* data_;
    size_t size_;

public:
    // 普通构造
    explicit Buffer(size_t n) : data_(new int[n]()), size_(n)
    {
        cout << "[Buffer 构造] 分配 " << n << " 个 int，地址: " << data_ << endl;
    }

    // 析构
    ~Buffer()
    {
        cout << "[Buffer 析构] 释放地址: " << data_ << endl;
        delete[] data_;
    }

    // 拷贝构造 —— 深拷贝，分配新内存
    Buffer(const Buffer& other) : data_(new int[other.size_]), size_(other.size_)
    {
        std::copy(other.data_, other.data_ + size_, data_);
        cout << "[Buffer 拷贝构造] 深拷贝 " << size_ << " 个元素, "
             << other.data_ << " → " << data_ << endl;
    }

    // 拷贝赋值 —— 先清理自己，再深拷贝
    Buffer& operator=(const Buffer& other)
    {
        if (this != &other)
        {
            delete[] data_;
            size_ = other.size_;
            data_ = new int[size_];
            std::copy(other.data_, other.data_ + size_, data_);
            cout << "[Buffer 拷贝赋值] 深拷贝 " << size_ << " 个元素\n";
        }
        return *this;
    }

    // ★ 移动构造 —— 偷走资源，源对象置空
    // 参数是非 const 右值引用（因为要修改源对象）
    // noexcept 是必须的！否则标准库不敢用
    Buffer(Buffer&& other) noexcept : data_(other.data_), size_(other.size_)
    {
        // 偷走资源后，源对象必须能安全析构
        other.data_ = nullptr;
        other.size_ = 0;
        cout << "[Buffer 移动构造] 偷走资源，地址: " << data_ << endl;
    }

    // ★ 移动赋值 —— 先释放自己，再偷资源
    Buffer& operator=(Buffer&& other) noexcept
    {
        if (this != &other)
        {
            delete[] data_;           // 释放自己的旧资源
            data_ = other.data_;      // 偷走对方的资源
            size_ = other.size_;
            other.data_ = nullptr;    // 源对象置空
            other.size_ = 0;
            cout << "[Buffer 移动赋值] 偷走资源，地址: " << data_ << endl;
        }
        return *this;
    }

    // 自检移动赋值：为什么先判断 this != &other？
    // 防止有人写 x = std::move(x) —— 虽然罕见，但自赋值检查是标准做法

    void fill(int val)
    {
        for (size_t i = 0; i < size_; ++i) data_[i] = val;
    }

    void print() const
    {
        cout << "  Buffer(size=" << size_ << ", data=" << data_ << "): ";
        for (size_t i = 0; i < size_ && i < 5; ++i) cout << data_[i] << " ";
        cout << endl;
    }
};

void lesson4_move_constructor()
{
    cout << "\n========== 第 4 节：手写移动构造/移动赋值 ==========\n";

    // -------------------------------------------------------
    // 实验 1：移动构造
    // -------------------------------------------------------
    {
        cout << "\n--- 移动构造 ---\n";
        Buffer buf1(5);
        buf1.fill(7);
        buf1.print();

        Buffer buf2(std::move(buf1));   // 调用移动构造
        buf2.print();
        buf1.print();                   // data_ = nullptr, size_ = 0
    }

    // -------------------------------------------------------
    // 实验 2：移动赋值
    // -------------------------------------------------------
    {
        cout << "\n--- 移动赋值 ---\n";
        Buffer buf1(5);
        buf1.fill(7);

        Buffer buf2(3);
        buf2.fill(3);
        cout << "移动赋值前:\n";
        buf1.print();
        buf2.print();

        buf2 = std::move(buf1);         // 调用移动赋值
        cout << "移动赋值后:\n";
        buf1.print();                   // 源对象置空
        buf2.print();                   // 资源已转移
    }

    // -------------------------------------------------------
    // 实验 3：noexcept 的重要性
    // -------------------------------------------------------
    {
        cout << "\n--- noexcept 的重要性 ---\n";
        cout << "如果移动构造不标记 noexcept，vector 扩容时会用拷贝而不是移动！\n";
        cout << "验证: " << std::boolalpha
             << std::is_nothrow_move_constructible<Buffer>::value << endl;
        // true → vector 扩容时会用移动，性能好
    }
}

// ============================================================
// 第 5 节：用 GDB 观察资源转移
// ============================================================
// GDB 实验步骤：
//   1. 在移动构造函数入口设断点
//   2. 用 print 查看源对象和目标的 data_ 指针
//   3. 观察移动完成后源对象 data_ 变为 nullptr
//   4. 用 bt 查看谁触发了移动
// ============================================================

void lesson5_gdb_move()
{
    cout << "\n========== 第 5 节：GDB 观察资源转移 ==========\n";

    Buffer a(10);
    a.fill(99);
    a.print();

    cout << "\n执行 Buffer b = std::move(a);\n";
    // GDB 操作：
    // (gdb) break Buffer::Buffer(Buffer&&)
    // (gdb) run
    // (gdb) print a.data_        ← 源对象的指针
    // (gdb) print this->data_    ← 当前正在构造的对象的指针（进入移动构造后）
    // (gdb) next                   走过偷资源三行
    // (gdb) print a.data_        ← 已经变成 nullptr
    // (gdb) bt                   ← 看谁触发了移动
    Buffer b = std::move(a);

    cout << "\n移动后:\n";
    a.print();    // data_ = nullptr
    b.print();    // data_ 现在是原来 a 的指针（资源已转移）
}

// ============================================================
// 第 6 节（综合）：转发参数给构造函数 —— emplace 模式
// ============================================================
// emplace_back 的原理：把参数完美转发给构造函数，直接在容器的内存上构造对象
// 代替 push_back：push_back 需要先构造临时对象再拷贝/移动
// ============================================================

struct Person
{
    string name;
    int age;

    Person(const string& n, int a) : name(n), age(a)
    {
        cout << "[Person 构造] " << name << ", " << age << endl;
    }

    Person(string&& n, int a) : name(std::move(n)), age(a)
    {
        cout << "[Person 移动构造] " << name << ", " << age << endl;
    }

    Person(const Person& other) : name(other.name), age(other.age)
    {
        cout << "[Person 拷贝构造] " << name << endl;
    }

    Person(Person&& other) noexcept : name(std::move(other.name)), age(other.age)
    {
        cout << "[Person 移动构造] " << name << endl;
    }
};

// 模拟 emplace_back：完美转发参数给构造函数
template<typename T, typename... Args>
T my_emplace(Args&&... args)
{
    // 用 forward<Args> 转发每个参数，保持各自的值类别
    cout << "my_emplace:";
    return T(std::forward<Args>(args)...);
}

void lesson6_emplace_pattern()
{
    cout << "\n========== 第 6 节：转发参数给构造函数 ==========\n";

    string name = "张三";

    cout << "传左值 name:\n";
    auto p1 = my_emplace<Person>(name, 20);    // name 被转发为左值引用

    cout << "\n传右值 string(\"李四\"):\n";
    auto p2 = my_emplace<Person>(string("李四"), 25);  // 右值被转发为右值 → 走移动
}

// -----------------------------------------------------------
// main
// -----------------------------------------------------------

int main()
{
    SetConsoleOutputCP(CP_UTF8);

    lesson1_move_is_just_cast();
    lesson2_universal_reference();
    lesson3_perfect_forwarding();
    lesson4_move_constructor();
    lesson5_gdb_move();
    lesson6_emplace_pattern();

    cout << "\n========== 程序正常结束 ==========\n";
    return 0;
}

// ============================================================
// 今日要点总结
// ============================================================
//
// std::move：
//   - 本质是 static_cast<T&&>，不移动任何数据
//   - 真正的移动由移动构造/移动赋值完成
//   - move 后源对象处于"有效但未指定"状态（可析构、可赋值）
//
// 万能引用 T&&：
//   - T 是模板参数且需要推导 → T&& 是万能引用
//   - 传入左值 → T = X&，引用折叠：X& + && = X&
//   - 传入右值 → T = X，T&& = X&&
//   - 具体类型+&& → 纯右值引用，不是万能引用
//
// std::forward<T>：
//   - 根据 T 判断原始参数的值类别
//   - T = X& → forward 返回左值引用
//   - T = X  → forward 返回右值引用
//   - move 是无条件转右值，forward 是有条件转发
//
// 手写移动构造/移动赋值：
//   - 参数 T&&（非 const）← 要修改源对象
//   - 标记 noexcept ← 标准库容器扩容时才会用移动
//   - 偷完后源对象指针置空 ← 保证安全析构
// ============================================================
