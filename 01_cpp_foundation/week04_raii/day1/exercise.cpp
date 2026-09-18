#include<iostream>
#include<vector>
#include<string>
#include<windows.h>

using std::cout;
using std::endl;
using std::vector;
using std::string;

// ============================================================
// Day 1 练习需求清单
// ============================================================
// 请逐条完成以下需求，不要看 example.cpp。
// 遇到报错先自己排查，实在不会再看 example.cpp 对照。
// ============================================================

// -----------------------------------------------------------
// 需求 1：写一个函数模板 myMax
// -----------------------------------------------------------
// 要求：
//   - 函数名：myMax
//   - 模板参数：typename T
//   - 参数：两个 const T& a, const T& b
//   - 返回：T（较大的那个）
//   - 如果 a > b 返回 a，否则返回 b
//
// 在 test_myMax() 中测试：
//   - myMax(3, 5)                   （自动推导 T=int）
//   - myMax(3.14, 2.71)             （自动推导 T=double）
//   - myMax<double>(3, 5.0)         （显式指定 T=double，参数类型不同时用）

// TODO: 写 myMax 模板
template<typename T>
T myMax(const T& a,const T& b)
{
    if(a > b)
    return a;
    else
    return b;
}

void test_myMax()
{
    // TODO: 测试 myMax，至少 3 种调用
    int a = 1,b = 2;
    cout << "int " << myMax(a,b) << endl;
    double c = 2.2,d = 42.1;
    cout << "double " << myMax(c,d) << endl;
    string e = "fda",f = "jjj";
    cout << "string " << myMax(e,f) << endl;
    int g = 4;
    double h = 5.2;
    cout << "显式指定 " << myMax<int>(g,h) << endl;
}

// -----------------------------------------------------------
// 需求 2：写一个函数模板 mySwap
// -----------------------------------------------------------
// 要求：
//   - 函数名：mySwap
//   - 参数：两个 T& a, T& b（引用，才能交换外部变量）
//   - 不返回值
//   - 用临时变量 T temp 完成交换
//
// 在 test_mySwap() 中测试：
//   - 交换两个 int 变量
//   - 交换两个 string 变量
//   - 交换两个 double 变量

// TODO: 写 mySwap 模板
template<typename T>
void mySwap(T& a,T& b)
{
    T temp = a;
    a = b;
    b = temp;
}

void test_mySwap()
{
    // TODO: 测试 mySwap，至少 3 种类型
    int a = 1,b = 2;
    cout << "交换前 a = " << a << " b = "  << b << endl; 
    mySwap(a,b);
    cout << "交换后 a = " << a << " b = " << b << endl;
    double c = 2.2,d = 42.1;
    cout << "交换前 c = " << c << " d = "  << d << endl; 
    mySwap(c,d);
    cout << "交换后 c = " << c << " d = " << d << endl;
    string e = "fda",f = "jjj";
    cout << "交换前 e = " << e << " f = "  << f << endl;
    mySwap(e,f);
    cout << "交换后 e = " << e << " f = " << f << endl; 
}

// -----------------------------------------------------------
// 需求 3：写一个函数模板，打印任意 vector 的所有元素
// -----------------------------------------------------------
// 要求：
//   - 函数名：printVector
//   - 参数：const vector<T>& v
//   - 用范围 for 遍历并打印每个元素，用空格分隔
//   - 打印完后换行
//
// 在 test_printVector() 中测试：
//   - 打印 vector<int>
//   - 打印 vector<string>

// TODO: 写 printVector 模板

template<typename T>
void print(const vector<T>& temp)
{
    for(auto x:temp)
    {
        cout << x << " ";
    }
    cout << endl;
}

void test_printVector()
{
    // TODO: 测试 printVector
    vector<int> a{3,5,2,6,4,7,8,10};
    vector<string> b{"123","456","cg","wj"};
    cout << "int:";
    print(a);
    cout << "string: ";
    print(b); 
}

// -----------------------------------------------------------
// 需求 4：写一个函数模板，统计 vector 中 >= 阈值的元素个数
// -----------------------------------------------------------
// 要求：
//   - 函数名：countGreaterEqual
//   - 参数：const vector<T>& v, T threshold
//   - 返回：int（满足条件的个数）
//   - 遍历 v，对每个 >= threshold 的元素计数
//
// 在 test_countGreaterEqual() 中测试：
//   - countGreaterEqual({1,3,5,7,9}, 5) → 返回 3
//   - countGreaterEqual({1.1, 2.2, 3.3, 4.4}, 3.0) → 返回 2

// TODO: 写 countGreaterEqual 模板
template<typename T>
int count(const vector<T>& temp,T threshold)
{
    int count = 0;
    for(auto x:temp)
    {
        if(x >= threshold)
        count++;
    }
    return count;
}

void test_countGreaterEqual()
{
    // TODO: 测试 countGreaterEqual
    vector<int> a{1,3,5,7,9};
    cout << "int: " << count(a,5) << endl;
    vector<double> b{1.1,2.2,3.3,4.4};
    cout << "double: " << count(b,3.0) << endl;
}

// -----------------------------------------------------------
// 需求 5（综合）：写一个函数模板 findMax
// -----------------------------------------------------------
// 要求：
//   - 函数名：findMax
//   - 参数：const vector<T>& v
//   - 返回：T（vector 中的最大值）
//   - 假设 vector 非空
//   - 用 myMax（需求 1 的模板）逐个比较
//
// 在 test_findMax() 中测试：
//   - findMax({1, 5, 3, 9, 2}) → 返回 9
//   - findMax({3.14, 1.41, 2.71}) → 返回 3.14
//   - findMax({string("abc"), string("abd"), string("aaa")}) → 返回 "abd"

// TODO: 写 findMax 模板
template<typename T>
T findMax(const vector<T>& temp)
{
    T a = temp[0];
    for(size_t i = 1;i < temp.size();i++)
    {
        a = myMax(temp[i],a);
    }
    return a;
}

void test_findMax()
{
    // TODO: 测试 findMax
    cout << "int: " << findMax(vector<int>{1, 5, 3, 9, 2}) << endl;
    cout << "double: " << findMax(vector<double>{3.14, 1.41, 2.71}) << endl;
    cout << "string: " << findMax(vector<string>{string("abc"), string("abd"), string("aaa")}) << endl;
}

// -----------------------------------------------------------
int main()
{
    SetConsoleOutputCP(CP_UTF8);

    //test_myMax();
    //test_mySwap();
    //test_printVector();
    //test_countGreaterEqual();
    test_findMax();

    return 0;
}
