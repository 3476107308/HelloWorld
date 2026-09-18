#include<iostream>
#include<string>
#include<windows.h>

using std::cout;
using std::endl;
using std::string;

// ============================================================
// Day 3 练习需求清单
// ============================================================
// 请逐条完成以下需求，不要看 example.cpp。
// 遇到报错先自己排查，实在不会再看 example.cpp 对照。
// ============================================================

// -----------------------------------------------------------
// 需求 1：解释模板为什么不能分文件编译
// -----------------------------------------------------------
// 用自己的话，在 main() 开头打印一段说明，解释：
//   为什么类模板的声明和实现不能像普通类一样分离到 .h 和 .cpp？
//
// 提示关键词：编译时机、具体类型、链接错误

void test_为什么不能分文件()
{
    cout << "========== 需求 1：模板为什么不能分文件编译 ==========\n";
    cout << "调用模板时才知道T的实际类型，此时需要有实现才能生成机器码\n如果实现在.cpp文件中 调用处（.h文件）就只有声明没有实现 不能生成机器码\n两边都不生成机器码，链接时就找不到函数定义\n";
    // TODO: 用 3-5 行 cout 输出你的理解
}

// -----------------------------------------------------------
// 需求 2：写一个类模板 Array<T> —— 基本功能
// -----------------------------------------------------------
// 要求：
//   - 类名：Array
//   - 私有成员：
//       T* data_       （指向堆上数组的指针）
//       size_t size_   （当前元素个数）
//       size_t cap_    （容量）
//
//   必须实现的函数（类内实现即可）：
//   - ① 构造函数 Array(size_t capacity = 4)
//       在堆上分配 new T[capacity]，size_=0, cap_=capacity
//
//   - ② 析构函数 ~Array()
//       打印 "[析构] 释放 N 个元素"，然后 delete[] data_
//
//   - void push(const T& value)
//       如果 size_ >= cap_，扩容为 2 倍
//       data_[size_] = value; size_++;
//
//   - T& operator[](size_t index)
//       返回 data_[index]
//
//   - size_t size() const     返回 size_
//   - size_t capacity() const 返回 cap_
//
//   私有函数：
//   - void resize(size_t newCap)
//       分配新数组，逐元素拷贝旧数据，释放旧数组，更新 cap_
//
// 在 test_Array() 中测试：
//   - Array<int>(2)，push 6 个数字，观察扩容
//   - 用 operator[] 遍历打印所有元素

// TODO: 写 Array 类模板
template<typename T>
class Array
{
private:
    T* data_;
    size_t size_;
    size_t cap_;
    void resize(size_t newcap)
    {
        T* newdata = new T[newcap];
        for(size_t i = 0;i < size_;i++)
        {
            newdata[i] = data_[i];
        }
        delete[] data_;
        data_ = newdata;
        newdata = nullptr;
        cap_ = newcap;
    }
public:
    Array(size_t capacity = 4):cap_(capacity)
    {
        data_ = new T[cap_];
        size_ = 0;
    }
    ~Array()
    {
        cout << "[析构] 释放了" << size_ << "个元素\n";
        delete[] data_; 
    }
    void push(const T&  value)
    {
        if(size_ >= cap_)
        resize(2 * cap_);
        data_[size_] = value;
        size_++;
    }
    T& operator[](size_t index)
    {
        return data_[index];
    }
    const T& operator[](size_t index)const
    {
        return data_[index];
    }
    const size_t size()const
    {
        return size_;
    }
    const size_t cap()const
    {
        return cap_;
    }
    T* data()
    {
        return data_;
    }
    Array(const Array& other):size_(other.size_),cap_(other.cap_)
    {
        data_ = new T[cap_];
        for(size_t i = 0; i < other.size_;i++)
        {
            data_[i] = other.data_[i];
        }
        cout << "[拷贝构造] 复制了" << size_ << "个元素\n";
    }
    Array& operator=(const Array& other)
    {
        if(this == &other)
        return *this;

        delete[] data_;
        data_ = new T[other.cap_];
        for(size_t i = 0;i < other.size_;i++)
        {
            data_[i] = other.data_[i];
        }
        size_ = other.size_;
        cap_ = other.cap_;
        cout << "[拷贝赋值] 复制了" << size_ << "个元素\n";
        return *this;
    }
    Array(Array&& other)noexcept:data_(std::move(other.data_)),size_(other.size_),cap_(other.cap_)
    {
        other.data_ = nullptr;
        other.size_ = 0;
        other.cap_ = 0;
        cout << "[移动构造] 偷走了" << size_ << "个元素\n";
    }
    Array& operator=(Array&& other)noexcept
    {
        if(this == &other)
        return *this;

        delete[] data_;
        data_ = other.data_;
        cap_ = other.cap_;
        size_ = other.size_;

        other.data_ = nullptr;
        other.cap_ = 0;
        other.size_ = 0;

        cout << "[移动赋值] 偷走了" << size_ << "个元素\n";
        return *this;
    }
    void show()const
    {
        for(size_t i = 0;i < size_;i++)
        {
            cout << data_[i] << " ";
        }
        cout << endl;
    }
};
void test_Array()
{
    // TODO: 测试 Array 基本功能 
    Array<int> a(4);
    a.push(1);
    a.push(3);
    a.push(10);
    cout << "a的第2个元素" << a[1] << endl;
    cout << "遍历a\n";
    a.show();
    cout << endl;
    cout << "扩容前" << (void*)a.data() << endl;
    a.push(5);
    a.push(13);
    cout << "扩容后" << (void*)a.data() << endl;
}

// -----------------------------------------------------------
// 需求 3：给 Array<T> 补全 Rule of Five
// -----------------------------------------------------------
// 在需求 2 的基础上，加上：
//
//   - ③ 拷贝构造 Array(const Array& other)
//       深拷贝：分配新数组，逐个复制元素
//       打印 "[拷贝构造] 复制了 N 个元素"
//
//   - ④ 拷贝赋值 operator=(const Array& other)
//       先判断自赋值（if this == &other）
//       释放旧资源，分配新数组，逐个复制
//       打印 "[拷贝赋值] 复制了 N 个元素"
//
//   - ⑤ 移动构造 Array(Array&& other) noexcept
//       偷走 other 的 data_/size_/cap_
//       把 other.data_ 置为 nullptr，other.size_/cap_ 置为 0
//       打印 "[移动构造] 偷走了 N 个元素"
//
//   - ⑥ 移动赋值 operator=(Array&& other) noexcept
//       自赋值检查、释放旧资源、偷新资源、置空源对象

// TODO: 补全 Array 的 Rule of Five

void test_RuleOfFive()
{
    // TODO: 验证深拷贝（修改副本不影响原数组）
    Array<int> a;
    a.push(1);
    a.push(2);
    Array<int> b(a);
    b.push(5);
    a.show();
    b.show();
    // TODO: 验证移动构造（源对象被掏空）
    Array<int> c(std::move(a));
    cout << "a.data_ " << (void*)a.data() << " a.cap_ " << a.cap() << " a.size " << a.size() << endl; 
}

// -----------------------------------------------------------
// 需求 4（综合）：用 Array<string> 存姓名列表
// -----------------------------------------------------------
// 要求：
//   - 创建一个 Array<string>，push 至少 5 个名字
//   - 遍历打印
//   - 拷贝到另一个 Array<string>
//   - 修改副本的第 0 个元素，打印原数组和副本
//     验证它们是独立的（深拷贝）

void test_综合()
{
    // TODO: 综合测试
    Array<string> a;
    a.push("陈");
    a.push("岗");
    a.push("喜");
    a.push("欢");
    a.push("王");
    a.push("静");
    a.show();
    Array<string> b = a;
    a[0] = "c";
    a.show();
    b.show();
}

// -----------------------------------------------------------
int main()
{
    SetConsoleOutputCP(CP_UTF8);

    test_为什么不能分文件();
    test_Array();
    test_RuleOfFive();
    test_综合();

    return 0;
}
