#include<iostream>
#include<vector>
#include<string>
#include<windows.h>

using std::cout;
using std::endl;
using std::vector;
using std::string;

// ============================================================
// Day 2 练习需求清单
// ============================================================
// 请逐条完成以下需求，不要看 example.cpp。
// 遇到报错先自己排查，实在不会再看 example.cpp 对照。
// ============================================================

// -----------------------------------------------------------
// 需求 1：写一个类模板 Pair<K, V>
// -----------------------------------------------------------
// 要求：
//   - 类名：Pair
//   - 两个私有成员：K key_ 和 V value_
//   - 构造函数：Pair(K key, V value)，用初始化列表给 key_ 和 value_ 赋值
//   - getKey() const → 返回 key_
//   - getValue() const → 返回 value_
//   - setKey(K key) → 修改 key_
//   - setValue(V value) → 修改 value_
//
// 在 test_Pair() 中测试：
//   - Pair<int, string> p1(1001, "张三") → 打印学号和姓名
//   - Pair<string, double> p2("C++", 98.5) → 打印科目和分数
//   - 用 setValue 修改 p2 的分数，再次打印验证

// TODO: 写 Pair 类模板
template<typename T1,typename T2>
class Pair
{
private:
    T1 key_;
    T2 value_;
public:
    Pair(T1 key,T2 value):key_(key),value_(value){}
    T1 getkey()const{return key_;}
    T2 getvalue()const{return value_;}
    void setkey(T1 key){key_ = key;}
    void setvalue(T2 value){value_ = value;}
};


void test_Pair()
{
    // TODO: 测试 Pair
    Pair<int,string> p1(1001,"张三");
    cout << p1.getkey() << " " << p1.getvalue() << endl;
    Pair<string,double> p2("C++",98.5);
    cout << p2.getkey() << " " << p2.getvalue() << endl; 
    p2.setvalue(22);
    cout << p2.getkey() << " " << p2.getvalue() << endl; 
}

// -----------------------------------------------------------
// 需求 2：写一个类模板 Container<T>
// -----------------------------------------------------------
// 要求：
//   - 类名：Container
//   - 私有成员：vector<T> data_
//   - void add(const T& item) → 往 data_ 里 push_back
//   - T get(size_t index) const → 返回 data_[index]
//   - size_t size() const → 返回 data_.size()
//   - bool empty() const → 返回 data_.empty()
//
// 在 test_Container() 中测试：
//   - Container<int>，add 5 个数，遍历打印
//   - Container<string>，add 3 个字符串，遍历打印
//   - 打印 size 和 empty 的结果

// TODO: 写 Container 类模板
template<typename T>
class Container
{
private:
    vector<T> data_;
public:
    void add(const T& item)
    {
        data_.push_back(item);
    }
    T get(size_t index)const
    {
        return data_.at(index);
    }
    size_t size()const
    {
        return data_.size();
    }
    bool empty()const
    {
        return data_.empty(); 
    }
};

void test_Container()
{
    // TODO: 测试 Container
    Container<int> a;
    a.add(1),a.add(4),a.add(1),a.add(8),a.add(19);
    for(size_t i = 0;i < a.size();i++)
    {
        cout << a.get(i) << " ";
    }
    cout << endl;
    Container<string> b;
    b.add("cg"),b.add(" love "),b.add("wj");
    for(size_t i = 0;i < b.size();i++)
    {
        cout << b.get(i);
    }
    cout << endl;
}

// -----------------------------------------------------------
// 需求 3：类模板成员函数类外实现
// -----------------------------------------------------------
// 要求：
//   - 把需求 2 的 Container<T> 改成"类内声明 + 类外实现"
//   - 构造函数也移到类外
//   - 类外实现的部分写在这个文件里（不要分文件）
//
//  提示：类外实现的语法是
//        template<typename T>
//        返回类型 类名<T>::函数名(参数) { ... }

// TODO: 写 Container2 类模板（类外实现版本）
template<typename T>
class Container2
{
private:
    vector<T> data_;
public:
    void add(const T& item);
    T get(size_t index)const;
    size_t size()const;
    bool empty()const;
};

template<typename T>
void Container2<T>::add(const T& item)
{
    data_.push_back(item);
}
template<typename T>
T Container2<T>::get(size_t index)const
{
    return data_.at(index);
}
template<typename T>
size_t Container2<T>::size()const
{
    return data_.size();
}
template<typename T>
bool Container2<T>::empty()const
{
    return data_.empty();
}

void test_Container2()
{
    // TODO: 测试 Container2，和 Container 一样的测试
    Container2<int> a;
    a.add(1),a.add(4),a.add(1),a.add(8),a.add(19);
    for(size_t i = 0;i < a.size();i++)
    {
        cout << a.get(i) << " ";
    }
    cout << endl;
    Container2<string> b;
    b.add("cg"),b.add(" love "),b.add("wj");
    for(size_t i = 0;i < b.size();i++)
    {
        cout << b.get(i);
    }
    cout << endl;
}

// -----------------------------------------------------------
// 需求 4（综合）：写一个函数模板，打印任意 Container<T>
// -----------------------------------------------------------
// 要求：
//   - 函数名：printContainer
//   - template<typename T>
//   - 参数：const Container<T>& c
//   - 用 for 循环 + get(i) 遍历打印，空格分隔，最后换行
//
// 在 test_printContainer() 中测试：
//   - 用 Container<int> 和 Container<string> 分别调用 printContainer

// TODO: 写 printContainer 函数模板
template<typename T>
void printContainer(const Container<T>& item)
{
    for(size_t i = 0;i < item.size();i++)
    {
        cout << item.get(i) << " ";
    }
    cout << endl;
}

void test_printContainer()
{
    // TODO: 测试 printContainer
    Container<int> a;
    a.add(1),a.add(4),a.add(1),a.add(8),a.add(19);
    printContainer(a);
    
    Container<string> b;
    b.add("cg"),b.add(" love "),b.add("wj");
    printContainer(b);
   
}

// -----------------------------------------------------------
int main()
{
    SetConsoleOutputCP(CP_UTF8);

    test_Pair();
    test_Container();
    test_Container2();
    test_printContainer();

    return 0;
}
