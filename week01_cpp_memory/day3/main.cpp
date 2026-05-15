#include<iostream>
using namespace std;
//全局区：存放全局 静态变量 和常量（局部常量除外）
//代码区：只读
//栈区：存放局部变量 编译器自动释放内存
//堆区：程序员手动开辟和释放内存

int g_a = 10,g_b = 11;
const int g_c_a = 12,g_c_b = 13;

struct Test
{
    char c;  //1字节
    int i;   //4字节
    short s; //2字节
};

int main()
{
    int a = 15,b = 16;
    const int l_c_a = 17,l_c_b = 18;
    static int s_a = 19,s_b = 20;
    int* p = new int(10);
    cout << "全局区\n";
    cout << "全局变量 " << &g_a << " " << &g_b << endl;
    cout << "全局常量 " << &g_c_a << " " << &g_c_b << endl;
    cout << "静态变量 " << &s_a << " " << &s_b << endl;
    cout << "字符串常量 " << &"helloworld" << endl;
    cout << "栈区\n";
    cout << "局部变量 " << &a << " " << &b << endl;
    cout << "局部常量 " << &l_c_a << " " << &l_c_b << endl;
    cout << "堆区\n";
    cout << "堆区变量 " << p << endl;
    cout << "sizeof(Test) = " << sizeof(Test) << endl;
    //编译器为了效率  会对齐到 int（4字节）的3倍



    delete p;
    return 0;
}