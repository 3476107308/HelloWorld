## decltype类型推导
decltype(变量)   变量的值类型是啥 就推导为啥 保留const 和 &   
decltype(表达式) 表达式是左值 推导为 & 表达式是将亡值 推导为&&  表达式为右值 推导为值类型
例如
```cpp
int a;
decltype(a) b;//b是int类型
decltype((a)) c;//c是int& 类型
```
## const
const int*  常量指针 底层const  说明 指针指向的值不能修改 指针可以修改
int* const  指针常量 顶层const  说明 指针本身不能修改 可以修改指针指向的值

# 模板 迭代器
auto add(T a,U b)->decltype(a+b)  在不确定类型时使用
decltype(auto) 保留引用 auto 不保留引用
