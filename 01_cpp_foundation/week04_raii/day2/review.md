# 模板类
## 类模板和普通类编译方式的区别
类模板不支持文件分离
原因：普通类是在 编译.cpp文件时就生成机器码，模板类是调用时才根据具体类型生成机器码
链接：对于普通类 链接器能找到 .o文件里的函数   对于模板类 .cpp文件里没有 就不会生成代码 链接找不到

普通类：编译器看到 int add(int, int) 就直接编译出机器码，其他 .cpp文件 链接时能找到

类模板：编译器看到 template<typename T> T add(T, T)，不生成任何代码！因为它不知道 T 是什么，只有在调用 add(3, 5) 时，才生成一份 T=int 的版本，如果模板实现在 .cpp 里，别的文件 include .h文件时看不见实现，调用 add(3, 5) 时没法生成代码 → 链接报错！

## 类外实现的类模板成员函数
```cpp
template<typename T>
class Container
{
    void add(const T& item);
};



template<typename T>
void Container<T>::add(const T& item)
{

}


```