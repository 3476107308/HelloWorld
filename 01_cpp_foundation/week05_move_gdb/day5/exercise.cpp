// ============================================================
// Week05 Day5：std::move + std::forward 完美转发 — exercise.cpp
// ============================================================
// 编译：
//   g++ -std=c++17 -Wall -g -O0 exercise.cpp -o exercise.exe
//
// GDB 验证：
//   gdb ./exercise.exe
//   (gdb) break 需要验证的函数
//   (gdb) run
//   (gdb) print / ptype / next / bt
// ============================================================

#include <iostream>
#include <vector>
#include <string>
#include <utility>      // std::move, std::forward
#include <windows.h>
#include<algorithm>     //std::copy

using std::cout;
using std::endl;
using std::vector;
using std::string;

// ============================================================
// 需求清单（请逐条完成，每条都要用 GDB 验证）
// ============================================================

// -----------------------------------------------------------
// 需求 1：验证 std::move 只是 cast
// -----------------------------------------------------------
// ① 写两个函数：
//     void take_copy(string s)   — 按值传参，打印 "拷贝/移动后: s"
//     void take_lref(string& s)  — 左值引用传参
//     void take_rref(string&& s) — 右值引用传参
// ② 在 test_move_is_cast() 中：
//     - 创建 string s = "hello";
//     - 分别传入 s、move(s)、string("tmp")，观察哪个重载被调用
//     - 打印每次调用后 s 的值，观察 move 是否修改了 s
// ③ 回答：std::move(s) 之后 s 的值变了吗？为什么？
//        如果答案是没有，那 move 到底做了什么？

// TODO: 写 take_copy / take_lref / take_rref 三个函数
void take_copy(string s)
{
    cout << "拷贝/移动后: " << s << endl;
}
void take_lref(string& s)
{
    cout << "左值引用: " << s << endl;
}
void take_rref(string&& s)
{
    cout << "右值引用: " << s << endl;
}

// TODO: 写 test_move_is_cast() 函数
void  test_move_is_cast()
{
    string s = "hello";
    take_copy(s);          
    take_copy(move(s));    
    take_copy(string("tmp")); 

    take_lref(s);          
    // take_lref(move(s));      // 编译错误！右值不能绑到 string&
    // take_lref(string("tmp")); // 编译错误！同上

    // take_rref(s);             // 编译错误！左值不能绑到 string&&
    take_rref(move(s));          
    take_rref(string("tmp"));    
}

// -----------------------------------------------------------
// 需求 2：写一个带移动构造和移动赋值的 MyBuffer 类
// -----------------------------------------------------------
// ① 写类 MyBuffer：
//     - int* data_ 和 size_t size_ 两个成员
//     - 构造函数 MyBuffer(size_t n) 分配 n 个 int（值初始化为 0）
//     - 析构函数 ~MyBuffer() 释放 data_
//     - 拷贝构造 + 拷贝赋值（深拷贝，打印 "深拷贝"）
//     - ★ 移动构造（noexcept，偷资源，源指针置空，打印 "移动构造"）
//     - ★ 移动赋值（noexcept，先 delete 自己，再偷资源，源指针置空，打印 "移动赋值"）
//     - void fill(int val) 把所有元素填成 val
//     - void print() const 打印前 5 个元素和数据指针地址
// ② 在 test_mybuffer_move() 中：
//     - 创建 MyBuffer buf1(10)，fill(7)，打印
//     - 用 move(buf1) 调用移动构造创建 buf2，打印 buf1 和 buf2
//     - 创建 MyBuffer buf3(5)，fill(3)
//     - 用 move(buf3) 调用移动赋值给 buf2，打印 buf3 和 buf2
// ③ 用 GDB 在移动构造函数设断点：
//     (gdb) break MyBuffer::MyBuffer(MyBuffer&&)
//     (gdb) print other.data_   ← 源对象指针
//     (gdb) print other.size_
//     (gdb) next                 走过偷资源
//     (gdb) print other.data_   ← 应该变 nullptr
//     (gdb) print this->data_   ← 和原来的 other.data_ 相同
// ④ 回答：移动赋值时为什么要先 delete[] data_？
//        移动构造和移动赋值为什么要标记 noexcept？

// TODO: 写 MyBuffer 类
class MyBuffer
{
private:
    int* data_;
    size_t size_;
public:
    MyBuffer(size_t n):data_(new int[n]()),size_(n){}
    ~MyBuffer(){delete[] data_;}
    MyBuffer(const MyBuffer& other):size_(other.size_),data_(new int[other.size_])
    {
        std::copy(other.data_,other.data_+other.size_,this->data_);
    }
    MyBuffer& operator=(const MyBuffer& other)
    {
        if(this == &other) return *this;
        delete[] data_;
        data_ = new int[other.size_];
        std::copy(other.data_,other.data_+other.size_,this->data_);
        size_ = other.size_;
        return *this;
    }
    MyBuffer(MyBuffer&& other)noexcept:size_(other.size_),data_(other.data_)
    {
        other.data_ = nullptr;
        other.size_ = 0;
    }
    MyBuffer& operator=(MyBuffer&& other)noexcept
    {
        if(this == &other)return *this;
        delete[] data_;
        data_ = other.data_;
        size_ = other.size_;
        other.data_ = nullptr;
        other.size_  = 0;
        return *this;
    }
    void fill(int val)
    {
        for(size_t i = 0;i < size_;i++)
        {
            data_[i] = val;
        }
    }
    void print()const
    {
        for(size_t i = 0;i < 5;i++)
        {
            cout << data_[i] << " " << &data_[i];
            cout << endl;
        }
    }
};

// TODO: 写 test_mybuffer_move() 函数
void test_mybuffer_move()
{
    MyBuffer buf1(10);
    buf1.fill(7);
    buf1.print();
    MyBuffer buf2(std::move(buf1));
    buf1.print();
    buf2.print();
    MyBuffer buf3(5);
    buf3.fill(3);
    buf2 = std::move(buf3);
    buf2.print();
    buf3.print();
}

// -----------------------------------------------------------
// 需求 3：区分 std::move 和 std::forward
// -----------------------------------------------------------
// ① 写三个目标函数：
//     void target(int& x)        — 打印 "左值版本: x"
//     void target(const int& x)  — 打印 "只读版本: x"
//     void target(int&& x)       — 打印 "右值版本: x"
// ② 写一个用 move 的转发函数：
//     template<typename T>
//     void forward_by_move(T&& arg) { target(std::move(arg)); }
// ③ 写一个用 forward 的转发函数：
//     template<typename T>
//     void forward_by_forward(T&& arg) { target(std::forward<T>(arg)); }
// ④ 在 test_forward_vs_move() 中：
//     - 创建 int x = 42;
//     - 分别用 forward_by_move 和 forward_by_forward 传入 x
//     - 分别用 forward_by_move 和 forward_by_forward 传入 100
//     - 观察每次调用走了哪个 target 版本
// ⑤ 用表格总结：move 转发和 forward 转发的行为差异
// ⑥ 回答：什么时候应该用 move，什么时候应该用 forward？

// TODO: 写三个 target() 重载
void target(int& x)
{
    cout << "左值版本:" << x << endl;
}
void target(const int& x)
{
    cout << "只读版本:" << x << endl;
}
void target(int&& x)
{
    cout << "右值版本:" << x << endl;
}
// TODO: 写 forward_by_move() 模板函数
template<typename T>
void forward_by_move(T&& arg)
{
    target(move(arg));
}
// TODO: 写 forward_by_forward() 模板函数
template<typename T>
void forward_by_forward(T&& arg)
{
    target(forward<T>(arg));
}

// TODO: 写 test_forward_vs_move() 函数
void test_forward_vs_move()
{
    int x = 42;
    forward_by_move(x);
    forward_by_forward(x);

    forward_by_move(100);
    forward_by_forward(100);
}

// -----------------------------------------------------------
// 需求 4：用 GDB 验证 vector 扩容时触发移动
// -----------------------------------------------------------
// ① 创建一个包含 MyBuffer 的 vector（用上面写好的 MyBuffer 类）
// ② 在 test_vector_move() 中：
//     - 创建 vector<MyBuffer> v;
//     - 用 v.reserve(2) 预留 2 个容量
//     - 用 v.push_back(MyBuffer(5))  添加第一个元素（临时对象 → 移动）
//     - 用 v.push_back(MyBuffer(8))  添加第二个元素
//     - 用 v.push_back(MyBuffer(3))  添加第三个元素（触发扩容）
//     - 观察扩容时打印了什么——是"深拷贝"还是"移动构造"？
// ③ 如果移动构造不加 noexcept，把 noexcept 去掉后再编译运行
//    对比两次输出——扩容时分别走拷贝还是移动？
// ④ 用 GDB 在 push_back 前后查看 v.capacity() 的变化
//    （在 GDB 中 print v.capacity() 或者 print v.size()）
// ⑤ 回答：为什么标准库要求移动构造标记 noexcept？

// TODO: 写 test_vector_move() 函数


// -----------------------------------------------------------
// 需求 5（综合）：写一个工厂函数 make_pair_obj
// -----------------------------------------------------------
// ① 写一个简单的类 Pair：
//     - 有两个成员 string first_ 和 int second_
//     - 有一个两参数构造函数 Pair(const string& s, int v) 打印 "拷贝构造"
//     - 有一个两参数构造函数 Pair(string&& s, int v) 打印 "移动构造"
//     - 有一个成员函数 void print() const 打印两个成员
// ② 写一个工厂函数：
//     template<typename... Args>
//     Pair make_pair_obj(Args&&... args)
//     {
//         return Pair(std::forward<Args>(args)...);
//     }
// ③ 在 test_factory() 中：
//     - 创建 string name = "Alice";
//     - 用 make_pair_obj(name, 30) 创建 Pair（name 应该走拷贝）
//     - 用 make_pair_obj(string("Bob"), 25) 创建 Pair（临时 string 应该走移动）
//     - 用 make_pair_obj("Charlie", 20) 创建 Pair（字面量应该走移动）
//     - 每次创建后调用 print() 打印结果
// ④ 用 GDB 在 Pair 的两个构造函数各设一个断点
//    确认三次调用分别走的是拷贝还是移动构造
// ⑤ 回答：factory 函数里的 Args&&... 和 std::forward<Args>(args)...
//        分别起了什么作用？和直接传参有什么区别？

// TODO: 写 Pair 类


// TODO: 写 make_pair_obj() 工厂函数


// TODO: 写 test_factory() 函数


// -----------------------------------------------------------
// main
// -----------------------------------------------------------

int main()
{
    SetConsoleOutputCP(CP_UTF8);

    cout << "===== Week05 Day5 std::move + std::forward 练习 =====\n";

    // TODO: 写完每个 test 函数后取消注释对应的调用
    test_move_is_cast();
    test_mybuffer_move();
    // test_forward_vs_move();
    // test_vector_move();
    // test_factory();

    cout << "\n===== 练习完成 =====\n";
    cout << "别忘了用 GDB 验证每个需求！\n";
    return 0;
}
