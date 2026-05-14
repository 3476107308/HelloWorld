#include <iostream>
using namespace std;

// [修正] const 指针三种形式的正确理解：
// const int* p      : 指针常量 —— p 可以指向别处，但不能通过 p 修改值（*p = xx 不行）
// int* const p      : 常量指针       —— p 不能指向别处（p = xx 不行），但可以通过 p 修改值
// const int* const p: 指向常量的常量指针 —— p 不能指向别处，也不能通过 p 修改值
// [原来的注释写反了：const int* 不是"常量指针"，int* const 才是]
//记忆法则： const在前面 常量就在后面 叫指针常量 说明指向的值不能修改
//          const在后面 常量就在前面 叫常量指针 说明指针不能修改 

void swap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main()
{
    int a = 10;
    int* p = &a;

    // 区分 p、&p、*p
    cout << "a = " << a << endl;       // a 的值
    cout << "&a = " << &a << endl;     // a 的地址
    cout << "p = " << p << endl;       // p 的值（即 a 的地址）
    cout << "&p = " << &p << endl;     // [补充] p 自身的地址（与 &a 不同！）
    cout << "*p = " << *p << endl;     // p 指向的值（即 a 的值）

    // swap 测试
    int x = 3, y = 5;
    cout << "before swap: x=" << x << ", y=" << y << endl;
    swap(&x, &y);
    cout << "after  swap: x=" << x << ", y=" << y << endl;

    cout << "hello world" << endl;
    return 0;
}