#include<iostream>
#include<string>
#include<windows.h>

using std::cout;
using std::endl;
using std::string;

// ============================================================
// Day 3: 类模板深水区 —— 分文件问题 + 数组封装
// 对应视频: 黑马 C++ 类模板 P171-P175
// https://www.bilibili.com/video/BV1et411b73Z/?p=171
// ============================================================

// ============================================================
// Lesson 1: 为什么类模板不能分文件编写
// ============================================================
//
// 普通类：Student.h（声明）+ Student.cpp（实现）✅
// 类模板：Array.h（声明）+ Array.cpp（实现）❌ 链接错误！
//
// 原因：
//   编译器编译 Array.cpp 时，看到 template<typename T> — 不知道 T 是什么
//   → 不生成任何代码
//   编译器编译 main.cpp 时，看到 Array<int> — 需要 int 版本的实现
//   → 但 #include "Array.h" 里只有声明，实现被藏在 Array.cpp 里看不到
//   → 编译器说："我要生成 Array<int> 的代码，但不知道实现长什么样！"
//   → 链接错误：undefined reference to Array<int>::push(int)
//
// 解决办法：
//   方案① 所有代码放头文件（最常用）：声明+实现都写 Array.h 里
//   方案② 在 .cpp 末尾显式实例化：template class Array<int>;（不灵活）
//   方案③ 把 .cpp 改后缀为 .tpp 或 .hpp，在 .h 末尾 #include 它
//
// 本周统一用方案①：声明和实现都放在类内，或同一个 .h 里

// ============================================================
// Lesson 2: 类模板与继承
// ============================================================

// 先定义一个简单的 Pair，供 Lesson 2 继承示例使用
template<typename T1, typename T2>
class Pair
{
protected:
    T1 first_;
    T2 second_;
public:
    Pair(T1 a, T2 b) : first_(a), second_(b) {}
};

// 方式①：子类指定父类的模板参数（父类变成普通类）
class IntPair : public Pair<int, int>   // 父类指定了 T1=int, T2=int
{
public:
    IntPair(int a, int b) : Pair<int, int>(a, b) {}
    // 这个子类只能存两个 int，没有模板灵活性，但写法简单
};

// 方式②：子类继续模板化（灵活，常用）
template<typename T>
class Triple : public Pair<T, T>    // 父类用 T 实例化
{
private:
    T third_;
public:
    Triple(T a, T b, T c) : Pair<T, T>(a, b), third_(c) {}
    T getThird() const { return third_; }
};

// ============================================================
// Lesson 3: Array<T> —— 手写动态数组类模板
// ============================================================
//
// 这就是第 1 周 IntArray 的模板化版本。
// 把 int 类型换成 T，支持 Array<int>、Array<string>、Array<double> 等。
//
// 实现 Rule of Five（五个特殊函数）：
//   ① 构造
//   ② 析构
//   ③ 拷贝构造（深拷贝）
//   ④ 拷贝赋值（深拷贝 + 自赋值检查）
//   ⑤ 移动构造（偷资源）
//   ⑥ 移动赋值（偷资源）

template<typename T>
class Array
{
private:
    T* data_;        // 指向堆上数组的指针
    size_t size_;    // 元素个数
    size_t cap_;     // 容量

public:
    // ① 构造函数：分配初始容量
    Array(size_t capacity = 4)
        : data_(new T[capacity]), size_(0), cap_(capacity)
    {
        cout << "[构造] 容量 " << cap_ << endl;
    }

    // ② 析构函数：释放堆内存
    ~Array()
    {
        cout << "[析构] 释放 " << size_ << " 个元素\n";
        delete[] data_;
    }

    // ③ 拷贝构造：深拷贝
    //    每次扩容旧元素都要重新拷贝 → 值语义的代价（Day5 学过）
    Array(const Array& other)
        : data_(new T[other.cap_]), size_(other.size_), cap_(other.cap_)
    {
        for (size_t i = 0; i < size_; i++)
            data_[i] = other.data_[i];     // T 类型的 =，对 int 就是普通赋值，对 string 就是深拷贝
        cout << "[拷贝构造] 复制了 " << size_ << " 个元素\n";
    }

    // ④ 拷贝赋值：深拷贝 + 自赋值保护
    Array& operator=(const Array& other)
    {
        if (this == &other) return *this;  // 自赋值检查

        delete[] data_;                    // 释放旧资源
        data_ = new T[other.cap_];
        size_ = other.size_;
        cap_ = other.cap_;
        for (size_t i = 0; i < size_; i++)
            data_[i] = other.data_[i];

        cout << "[拷贝赋值] 复制了 " << size_ << " 个元素\n";
        return *this;
    }

    // ⑤ 移动构造：偷资源，挖空源对象
    Array(Array&& other) noexcept
        : data_(other.data_), size_(other.size_), cap_(other.cap_)
    {
        other.data_ = nullptr;    // 源对象指针置空
        other.size_ = 0;
        other.cap_ = 0;
        cout << "[移动构造] 偷走了 " << size_ << " 个元素\n";
    }

    // ⑥ 移动赋值
    Array& operator=(Array&& other) noexcept
    {
        if (this == &other) return *this;

        delete[] data_;
        data_ = other.data_;
        size_ = other.size_;
        cap_ = other.cap_;

        other.data_ = nullptr;
        other.size_ = 0;
        other.cap_ = 0;

        cout << "[移动赋值] 偷走了 " << size_ << " 个元素\n";
        return *this;
    }

    // ---------- 成员函数 ----------

    // 添加元素（可能触发扩容）
    void push(const T& value)
    {
        if (size_ >= cap_)
            resize(cap_ * 2);             // GCC 下默认 2 倍扩容
        data_[size_++] = value;
    }

    // 下标访问
    T& operator[](size_t index) { return data_[index]; }
    const T& operator[](size_t index) const { return data_[index]; }

    // 获取信息
    size_t size()     const { return size_; }
    size_t capacity() const { return cap_; }
    bool   empty()    const { return size_ == 0; }

    // 获取底层指针（类似 vector::data()）
    T* data() { return data_; }

private:
    // 扩容：分配新数组 → 拷贝旧数据 → 释放旧数组
    void resize(size_t newCap)
    {
        cout << "[扩容] " << cap_ << " -> " << newCap << endl;
        T* newData = new T[newCap];
        for (size_t i = 0; i < size_; i++)
            newData[i] = data_[i];        // 每个元素逐个拷贝
        delete[] data_;
        data_ = newData;
        cap_ = newCap;
    }
};

void lesson3_Array()
{
    cout << "========== Lesson 3: Array<T> 类模板 ==========\n\n";

    // Array<int>
    cout << "--- Array<int> ---\n";
    Array<int> ai(2);                     // 初始容量 2
    for (int i = 1; i <= 6; i++)
    {
        ai.push(i * 10);
        cout << "  push " << i*10 << " → size=" << ai.size()
             << " cap=" << ai.capacity() << endl;
    }
    cout << "遍历：";
    for (size_t i = 0; i < ai.size(); i++)
        cout << ai[i] << " ";
    cout << endl;

    // Array<string> —— 同一个模板，支持不同类型
    cout << "\n--- Array<string> ---\n";
    Array<string> as;
    as.push("张三");
    as.push("李四");
    as.push("王五");
    for (size_t i = 0; i < as.size(); i++)
        cout << as[i] << " ";
    cout << endl;

    // 拷贝
    cout << "\n--- 拷贝 Array<int> ---\n";
    Array<int> ai2 = ai;                  // 拷贝构造
    cout << "原数组[0]=" << ai[0] << " 副本[0]=" << ai2[0] << endl;
    ai2[0] = 999;
    cout << "副本[0]=999 后，原数组[0]=" << ai[0] << " (不受影响)\n";

    // 移动
    cout << "\n--- 移动 Array<int> ---\n";
    Array<int> ai3 = std::move(ai2);       // 移动构造
    cout << "ai2 size=" << ai2.size() << " (被掏空)\n";
    cout << "ai3 size=" << ai3.size() << " (拥有原数据)\n";
}

// ============================================================
// Lesson 4: 模板化的优势 —— 对比
// ============================================================

// 没有模板时，想支持 int 和 string，要写两份几乎一样的代码：
// class IntArray { ... };      // 100 行
// class StringArray { ... };   // 也是 100 行，几乎一样

// 有了模板：template<typename T> class Array { ... };
// Array<int>、Array<string>、Array<double> 一份代码搞定

void lesson4_对比()
{
    cout << "\n========== Lesson 4: Array<不同类型> 对比 ==========\n\n";

    Array<int>    a1;  a1.push(10);   a1.push(20);
    Array<double> a2;  a2.push(3.14); a2.push(2.71);
    Array<string> a3;  a3.push("C++"); a3.push("模板");

    cout << "Array<int>:    "; for (size_t i=0; i<a1.size(); i++) cout << a1[i] << "  ";
    cout << "\nArray<double>: "; for (size_t i=0; i<a2.size(); i++) cout << a2[i] << "  ";
    cout << "\nArray<string>: "; for (size_t i=0; i<a3.size(); i++) cout << a3[i] << "  ";
    cout << endl;

    // 同一份 Array 模板代码，生成了三个不同类型的类
    // 编译器在编译时自动生成：
    //   Array<int>    版本（T 替换为 int）
    //   Array<double> 版本（T 替换为 double）
    //   Array<string> 版本（T 替换为 string）
}

// ============================================================
int main()
{
    SetConsoleOutputCP(CP_UTF8);

    lesson3_Array();
    lesson4_对比();

    return 0;
}
