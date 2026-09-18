#include<iostream>
#include<vector>
#include<string>
#include<windows.h>

using std::cout;
using std::endl;
using std::vector;
using std::string;

// ============================================================
// Day 2: 类模板
// 对应视频: 黑马 C++ 类模板 P167-P170
// https://www.bilibili.com/video/BV1et411b73Z/?p=167
// ============================================================

// ============================================================
// Lesson 1: 第一个类模板 —— Pair<K, V>
// ============================================================
//
// 类模板和函数模板一样，把类型参数化。
// template<typename K, typename V> 意思是：
//   K — 第一个值的类型（Key，键）
//   V — 第二个值的类型（Value，值）
// 调用时 Pair<int, string> 让 K=int, V=string

template<typename K, typename V>
class Pair
{
private:
    K key_;
    V value_;

public:
    // 构造：把两个值存进去
    Pair(K key, V value) : key_(key), value_(value) {}

    K getKey()   const { return key_;   }
    V getValue() const { return value_; }

    void setKey(K key)     { key_ = key;     }
    void setValue(V value) { value_ = value; }
};

void lesson1_Pair()
{
    cout << "========== Lesson 1: Pair 类模板 ==========\n\n";

    Pair<int, string> p1(1001, "张三");
    cout << "学号: " << p1.getKey() << " 姓名: " << p1.getValue() << endl;

    Pair<string, double> p2("数学", 92.5);
    cout << "科目: " << p2.getKey() << " 分数: " << p2.getValue() << endl;

    // K 和 V 可以是相同类型
    Pair<string, string> p3("name", "李四");
    cout << p3.getKey() << " = " << p3.getValue() << endl;

    // 使用时必须指定具体类型：Pair<int, string>
    // 不能只写 Pair p1(1001, "张三");（C++17 前不行）
}

// ============================================================
// Lesson 2: Container<T> —— 用 vector 做内部存储
// ============================================================

template<typename T>
class Container
{
private:
    vector<T> data_;   // 内部用 vector 存数据

public:
    // 添加元素
    void add(const T& item)
    {
        data_.push_back(item);
    }

    // 按位置获取（有越界检查）
    T get(size_t index) const
    {
        return data_.at(index);  // at() 越界时会抛异常
    }

    // 获取元素个数
    size_t size() const
    {
        return data_.size();
    }

    // 判断是否为空
    bool empty() const
    {
        return data_.empty();
    }
};

void lesson2_Container()
{
    cout << "\n========== Lesson 2: Container 类模板 ==========\n\n";

    Container<int> ci;
    ci.add(10);
    ci.add(20);
    ci.add(30);
    cout << "Container<int>: ";
    for (size_t i = 0; i < ci.size(); i++)
        cout << ci.get(i) << " ";
    cout << "\nsize = " << ci.size() << endl;

    Container<string> cs;
    cs.add("hello");
    cs.add("world");
    cout << "Container<string>: ";
    for (size_t i = 0; i < cs.size(); i++)
        cout << cs.get(i) << " ";
    cout << "\nsize = " << cs.size() << endl;
}

// ============================================================
// Lesson 3: 成员函数类外实现
// ============================================================
//
// 类模板的成员函数如果放在类外实现，必须：
// ① 每个函数前加 template<typename T>
// ② 函数名前加 类名<T>::
// ③ 声明和实现必须放在同一个文件中（通常都在头文件）

template<typename T>
class Calculator
{
private:
    T a_, b_;

public:
    Calculator(T a, T b);   // 构造只声明
    T add() const;           // add 只声明
    T sub() const;           // sub 只声明

    // 简单函数可以直接在类内写（内联）
    T getA() const { return a_; }
};

// ↓ 类外实现构造：类名<T>::函数名(参数)
template<typename T>
Calculator<T>::Calculator(T a, T b) : a_(a), b_(b) {}

// ↓ 类外实现 add
template<typename T>
T Calculator<T>::add() const
{
    return a_ + b_;
}

// ↓ 类外实现 sub
template<typename T>
T Calculator<T>::sub() const
{
    return a_ - b_;
}

void lesson3_类外实现()
{
    cout << "\n========== Lesson 3: 成员函数类外实现 ==========\n\n";

    Calculator<int>    ci(20, 8);
    Calculator<double> cd(3.14, 1.0);

    cout << "Calculator<int>    : add = " << ci.add() << " sub = " << ci.sub() << endl;
    cout << "Calculator<double> : add = " << cd.add() << " sub = " << cd.sub() << endl;
}

// ============================================================
// Lesson 4: 类模板做函数参数
// ============================================================
//
// 三种方式：
// ① 指定具体类型：void func(Container<int>& c)          — 最死板
// ② 函数模板化：   template<typename T> void func(Container<T>& c) — 最常用
// ③ 整个类模板化： template<class C> void func(C& c)    — 最灵活

// 方式①：死板，只能接收 Container<int>
void printContainer_int(const Container<int>& c)
{
    for (size_t i = 0; i < c.size(); i++)
        cout << c.get(i) << " ";
    cout << endl;
}

// 方式②：常用，接收 Container<任意T>
template<typename T>
void printContainer(const Container<T>& c)
{
    for (size_t i = 0; i < c.size(); i++)
        cout << c.get(i) << " ";
    cout << endl;
}

void lesson4_做函数参数()
{
    cout << "\n========== Lesson 4: 类模板做函数参数 ==========\n\n";

    Container<int> ci;
    ci.add(1); ci.add(2); ci.add(3);

    Container<string> cs;
    cs.add("C++"); cs.add("模板");

    cout << "printContainer<int>: ";
    printContainer(ci);

    cout << "printContainer<string>: ";
    printContainer(cs);
}

// ============================================================
int main()
{
    SetConsoleOutputCP(CP_UTF8);

    lesson1_Pair();
    lesson2_Container();
    lesson3_类外实现();
    lesson4_做函数参数();

    return 0;
}
