# nullptr 和 NULL
nullptr 是std::nullptr_t类型 可以隐式转换到任何指针类型   NULL是int类型 作为实参传入是 nullptr调用的是指针   NULL调用的是0

# using 和 typedef 
using 别名 = 原来名
typedef 原来名 别名   
```cpp
typedef unordered_map<string,int> ScoreMap
```
给模板起别名时 只能用using 因为typedef编译不了

# 左值 右值 将亡值
左值是有地址的变量
右值是没有地址的临时值/表达式结果
将亡值是 原本有地址 但是被显式标记为马上要销毁   变量名还在 但是被当作右值处理 不能取地址
右值引用类型的变量本身是左值

# std::move
move不移动数据 只是类型转换 将左值转成右值引用 

实参是左值时 优先匹配& 然后匹配 const T&
实参是右值时 优先匹配&& 然后匹配const T&
实参是const &时 是万能实参 先匹配左值 再匹配右值 最后匹配const T&
const &是只读