// Week06 Day02：constexpr、=default、=delete 练习
// 说明：本文件只提供需求骨架，请独立完成 TODO。

#include <iostream>
#include <utility>

using std::cout;
using std::endl;

// TODO 1：实现 constexpr int cube(int value)
// 要求：返回 value 的三次方，并能够用于 static_assert。
constexpr int cube(int value)
{
    return value * value * value;
}

// TODO 2：完成 Config 类
// 要求：
// - 保存 port_ 和 timeout_ms_ 两个 int 字段。
// - 提供 constexpr 默认构造函数，默认端口 8080，默认超时 3000。
// - 提供 constexpr 两参数构造函数。
// - 提供 constexpr int port() const。
// - 提供 constexpr int timeout_ms() const。
class Config
{
public:
    // TODO：补充构造函数和访问函数
    constexpr Config():port_(8080),timeout_ms_(3000){}
    constexpr Config(int port,int timeout_ms):port_(port),timeout_ms_(timeout_ms){}
    constexpr int port() const{return port_;}
    constexpr int timeout_ms() const {return timeout_ms_;}
private:
    // TODO：补充字段和默认成员初始化
    int port_;
    int timeout_ms_;
};

// TODO 3：完成 NonCopyable 类
// 要求：
// - 默认构造函数使用 =default。
// - 拷贝构造函数使用 =delete。
// - 拷贝赋值运算符使用 =delete。
// - 可选：补充 noexcept 移动构造和移动赋值。
class NonCopyable
{
public:
    // TODO：补充特殊成员函数
    NonCopyable() = default;
    NonCopyable(const NonCopyable& other) = delete;
    NonCopyable& operator= (const NonCopyable& other) = delete;
    NonCopyable(NonCopyable&& other)noexcept = default;
    NonCopyable& operator= (NonCopyable&& other)noexcept = default;
};

void test_constexpr()
{
    // TODO：创建 constexpr int value = cube(3)。
    constexpr int value = cube(3);
    // TODO：用 static_assert 验证 value == 27。
    static_assert(value == 27);
}

void test_config()
{
    // TODO：创建一个 constexpr 默认 Config 和一个自定义 Config。
    constexpr Config a;
    constexpr Config b(8081,2000);
    // TODO：打印端口和超时时间。
    static_assert(a.port() == 8080);
    static_assert(b.port() == 8081);
    cout << a.port() << " " << a.timeout_ms() << endl;
    cout << b.port() << " " << b.timeout_ms() << endl;
}

void test_non_copyable()
{
    // TODO：创建 NonCopyable 对象并验证默认构造。
    NonCopyable a;
    // TODO：如果实现了移动操作，验证移动构造。
    NonCopyable b = std::move(a);
    // 注意：不要取消注释下面的非法复制代码，否则最终目标不能编译。
    // NonCopyable copied = original;
    // NonCopyable assigned;
    // assigned = original;
}

int main()
{
    cout << "===== Week06 Day02 exercise =====" << endl;
    test_constexpr();
    test_config();
    test_non_copyable();
    cout << "Complete the TODO items." << endl;
    return 0;
}
