## auto
auto 会值语义 只拷贝 丢失const 和 &   适用于需要拷贝的情况
auto&   保留const  适用于需要修改的情况
const auto&   只读     适用于只读不修改的情况  这时候不应拷贝 避免拷贝的额外消耗
auto&&   万能引用 适用于任何情况（左值 右值 const 非const）  适用于模板转发 和 auto&& lambda参数

# auto的用途
迭代器 省去较长的类型
lambda 是匿名类型 只能用auto来接

# {}来构造时 要小心使用auto 
```cpp
auto y = 42 //y是int类型   不会报错
auto z = {42} //z不是int类型 不会报错
auto x = {1,2}  //x不是int类型 也不是vetor<int>类型   不会报错
auto m{1,2} //会报错 不支持直接初始化
``` 