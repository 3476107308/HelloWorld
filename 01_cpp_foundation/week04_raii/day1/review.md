# 模板函数
## 模板函数和普通函数的区别
模板函数告诉编译器根据实参的参数类型来确定形参
普通函数的形参是在编译前确定好的
模板函数是模型 蓝图
普通函数是具体实体

## 3种不同场景的模板函数
```cpp
    //单类型参数（常见场景：比较、交换）
    template<typename T>
    void function1(T)
    {}

    //带比较器的排序
    template<typename T,typename compare>
    void function2(T,compare)
    {}

    //多类型参数+返回值推导
    template<typename T1,typename T2>
    auto function3(T1 a,T2 b)->decltype(a+b)
    { return  (a+b);}
```