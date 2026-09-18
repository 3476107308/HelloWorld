#include<iostream>
#include<vector>
#include<string>
#include<windows.h>

using std::cout;
using std::endl;
using std::vector;
using std::string;

// ============================================================
// Day 1: 函数模板
// 对应视频: 黑马 C++ 函数模板 P163-P166
// https://www.bilibili.com/video/BV1et411b73Z/?p=163
// ============================================================

// ============================================================
// Lesson 1: 为什么需要模板？
// ============================================================
//
// 没有模板时，想支持不同类型，只能写多份几乎一样的代码：

int max_int(int a, int b)        { return a > b ? a : b; }
double max_double(double a, double b) { return a > b ? a : b; }
string max_string(string a, string b) { return a > b ? a : b; }
//                    ↑ 除了类型不同，逻辑完全一样 → 代码重复

// 模板解决这个问题：用一个"类型占位符" T 代替具体类型
// T 不是真实类型，是"蓝图"——调用时编译器根据实际参数自动生成对应版本

// ============================================================
// Lesson 2: 第一个函数模板 —— myMax
// ============================================================

// template<typename T> 告诉编译器：下面这个函数里，T 是一个类型参数
// 调用 myMax(3, 5) 时，编译器推导出 T = int，生成一份 int 版本的代码
// 调用 myMax(3.14, 2.71) 时，推导出 T = double，再生成一份 double 版本
template<typename T>
T myMax(T a, T b)
{
    return a > b ? a : b;
}

// typename 和 class 在这里是完全等价的，都可以用：
// template<class T>
// T myMax(T a, T b) { return a > b ? a : b; }

void lesson2_myMax()
{
    cout << "========== Lesson 2: myMax 函数模板 ==========\n\n";

    // 自动推导：编译器根据参数类型推断 T
    cout << "myMax(3, 5)     = " << myMax(3, 5) << endl;        // T = int
    cout << "myMax(3.14, 2.71) = " << myMax(3.14, 2.71) << endl; // T = double
    cout << "myMax(\"abc\", \"abd\") = " << myMax(string("abc"), string("abd")) << endl; // T = string

    // 显式指定：强制 T 是什么类型
    cout << "myMax<int>(3, 5)   = " << myMax<int>(3, 5) << endl;
    cout << "myMax<double>(3, 5) = " << myMax<double>(3, 5) << endl;
    //        ↑ 3 和 5 先转换成 double，再比较
    // 什么时候必须显式指定？当参数类型不同时：
    // myMax(3, 5.0);  // ❌ 编译错误！T 是 int 还是 double？编译器不知道该推导哪个
    cout << "myMax<double>(3, 5.0) = " << myMax<double>(3, 5.0) << endl;  // ✅ 显式指定
}

// ============================================================
// Lesson 3: 函数模板 mySwap —— 引用参数
// ============================================================

// 模板参数 T 不仅可以用在返回值、参数类型，还可以用在引用上
// T& 表示"T 类型的引用"，这样才能真正交换外部的变量
template<typename T>
void mySwap(T& a, T& b)
{
    T temp = a;  // T 临时变量
    a = b;
    b = temp;
}

void lesson3_mySwap()
{
    cout << "\n========== Lesson 3: mySwap 函数模板 ==========\n\n";

    int x = 10, y = 20;
    cout << "交换前：x=" << x << " y=" << y << endl;
    mySwap(x, y);
    cout << "交换后：x=" << x << " y=" << y << endl;

    string s1 = "张三", s2 = "李四";
    cout << "交换前：s1=" << s1 << " s2=" << s2 << endl;
    mySwap(s1, s2);
    cout << "交换后：s1=" << s1 << " s2=" << s2 << endl;

    // mySwap 一个函数模板，对 int、string 都有效
    // 编译器自动为每种类型生成一份 swap 实现，你不需要手写两次
}

// ============================================================
// Lesson 4: 模板排序 mySort
// ============================================================

// 函数模板可以和 STL 容器一起用
// 这个 mySort 对任何支持 > 比较的类型都有效
template<typename T>
void mySort(vector<T>& v)
{
    // 简单冒泡排序演示（实际项目用 std::sort）
    for (size_t i = 0; i < v.size(); i++)
    {
        for (size_t j = 0; j < v.size() - i - 1; j++)
        {
            if (v[j] > v[j + 1])  // 用 > 比较，升序排列
            {
                T temp = v[j];
                v[j] = v[j + 1];
                v[j + 1] = temp;
            }
        }
    }
}

// 如果想支持降序，可以再定义一个带比较函数的模板
template<typename T, typename Compare>
void mySort(vector<T>& v, Compare comp)
{
    for (size_t i = 0; i < v.size(); i++)
    {
        for (size_t j = 0; j < v.size() - i - 1; j++)
        {
            if (comp(v[j + 1], v[j]))  // comp(v[j+1], v[j]) == true → 交换
            {
                T temp = v[j];
                v[j] = v[j + 1];
                v[j + 1] = temp;
            }
        }
    }
}

void lesson4_mySort()
{
    cout << "\n========== Lesson 4: mySort 函数模板 ==========\n\n";

    // 排序 int
    vector<int> vi{5, 2, 8, 1, 9};
    cout << "int 排序前：";
    for (auto x : vi) cout << x << " ";
    mySort(vi);
    cout << "\nint 排序后：";
    for (auto x : vi) cout << x << " ";
    cout << endl;

    // 排序 string
    vector<string> vs{"张三", "李四", "王五", "赵六"};
    cout << "string 排序前：";
    for (auto& s : vs) cout << s << " ";
    mySort(vs);
    cout << "\nstring 排序后：";
    for (auto& s : vs) cout << s << " ";
    cout << endl;

    // 自定义排序：用 lambda 按长度排序
    cout << "\n自定义排序（按字符串长度降序）：";
    mySort(vs, [](const string& a, const string& b) { return a.size() > b.size(); });
    for (auto& s : vs) cout << s << " ";
    cout << endl;
}

// ============================================================
// Lesson 5: 函数模板的局限性
// ============================================================

// 模板不是万能的。以下场景需要特别注意：

// ① 如果 T 不支持模板里用到的操作，编译会报错
//    例如 myMax 要求 T 支持 >（大于号比较）
//    如果你写 myMax(MyClass, MyClass)，但 MyClass 没有重载 operator>，编译报错

// ② 不同类型参数会导致推导冲突：
//    myMax(3, 5.0);  // ❌ T=int or T=double? 编译器报错

// ③ 函数模板可以写"特化版本"来为特定类型提供不同的实现：
// 例如：myMax 对 const char*（字符串字面量）做特殊处理
template<>
const char* myMax(const char* a, const char* b)
{
    // strcmp: <0 表示 a<b, >0 表示 a>b, ==0 表示相等
    return (strcmp(a, b) > 0) ? a : b;
}

void lesson5_局限性()
{
    cout << "\n========== Lesson 5: 函数模板的局限性 ==========\n\n";

    // 如果不特化，比较的是指针地址而不是字符串内容
    const char* s1 = "hello";
    const char* s2 = "world";
    cout << "myMax(const char*, const char*) 特化版：" << myMax(s1, s2) << endl;
    // 特化版会用 strcmp 比较内容，"world" 更大
}

// ============================================================
// 补充：auto + decltype —— C++14 返回值推导
// ============================================================

// C++14 起可以用 auto 做返回类型，编译器根据 return 语句推导：
template<typename T1, typename T2>
auto add(T1 a, T2 b)// 返回类型由 return 推导
{
    return a + b;      // 如果 a+b 是 double，返回值就是 double
}

void lesson6_auto返回值()
{
    cout << "\n========== 补充: auto 返回值推导 ==========\n\n";
    cout << "add(3, 5)     = " << add(3, 5) << endl;       // int + int = int
    cout << "add(3, 5.0)   = " << add(3, 5.0) << endl;     // int + double = double
    cout << "add(3.14, 2)  = " << add(3.14, 2) << endl;    // double + int = double
}

// ============================================================
int main()
{
    SetConsoleOutputCP(CP_UTF8);

    lesson2_myMax();
    lesson3_mySwap();
    lesson4_mySort();
    lesson5_局限性();
    lesson6_auto返回值();

    return 0;
}
