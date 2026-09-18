// Week06 Day02：constexpr、=default、=delete
// 目标：理解编译期计算、显式默认特殊成员函数和禁止拷贝。

#include <iostream>
#include <utility>

using std::cout;
using std::endl;

// constexpr 函数在参数是编译期常量时，可以在编译阶段计算结果。
constexpr int square(int value)
{
    return value * value;
}

// =default 让编译器生成默认实现，避免手写没有额外逻辑的函数。
class DefaultedRecord
{
public:
    DefaultedRecord() = default;
    DefaultedRecord(const DefaultedRecord&) = default;
    DefaultedRecord& operator=(const DefaultedRecord&) = default;

    explicit DefaultedRecord(int value) : value_(value) {}

    int value() const
    {
        return value_;
    }

private:
    int value_ = 0;
};

// constexpr 构造函数允许 Config 对象在编译期创建。
class Config
{
public:
    static constexpr int kDefaultPort = 8080;

    constexpr Config() = default;

    constexpr Config(int port, int timeout_ms)
        : port_(port), timeout_ms_(timeout_ms)
    {
    }

    constexpr int port() const
    {
        return port_;
    }

    constexpr int timeout_ms() const
    {
        return timeout_ms_;
    }

private:
    int port_ = kDefaultPort;
    int timeout_ms_ = 3000;
};

// =delete 会在编译期明确禁止复制操作。
class NonCopyable
{
public:
    NonCopyable() = default;
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;

    // 这里允许移动，说明“禁止复制”和“禁止移动”是两个独立决定。
    NonCopyable(NonCopyable&&) noexcept = default;
    NonCopyable& operator=(NonCopyable&&) noexcept = default;
};

void demonstrate_constexpr()
{
    constexpr int compile_time_value = square(5);
    static_assert(compile_time_value == 25);

    const int runtime_value = 6;
    cout << "constexpr square(5) = " << compile_time_value << endl;
    cout << "const square(runtime_value) = " << square(runtime_value) << endl;
}

void demonstrate_default()
{
    DefaultedRecord first(42);
    DefaultedRecord copied = first;
    DefaultedRecord assigned;
    assigned = first;

    cout << "defaulted copy value = " << copied.value() << endl;
    cout << "defaulted assignment value = " << assigned.value() << endl;
}

void demonstrate_config()
{
    constexpr Config compile_time_config;
    constexpr Config custom_config(9000, 1000);
    static_assert(compile_time_config.port() == Config::kDefaultPort);
    static_assert(custom_config.timeout_ms() == 1000);

    cout << "default port = " << compile_time_config.port() << endl;
    cout << "custom timeout = " << custom_config.timeout_ms() << " ms" << endl;
}

void demonstrate_delete()
{
    NonCopyable original;
    NonCopyable moved = std::move(original);
    (void)moved;

    cout << "NonCopyable can be default-constructed and moved." << endl;
    cout << "Copy construction and copy assignment are deleted." << endl;

    // 下面两行故意不能编译，只用于观察 =delete 的诊断信息；不要取消注释。
    // NonCopyable copied = original;
    // NonCopyable assigned; assigned = original;
}

int main()
{
    cout << "===== Week06 Day02 example =====" << endl;
    demonstrate_constexpr();
    demonstrate_default();
    demonstrate_config();
    demonstrate_delete();
    return 0;
}
