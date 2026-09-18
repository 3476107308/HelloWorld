# RAII
RAII是构造获取资源 析构释放资源 资源可以是内存 文件 锁
智能指针是RAII的一种应用 对应资源是内存

# unique_ptr
unique_ptr离开作用域后会自动delete 无需手动释放
unique_ptr是独占的 不能拷贝 只能移动
对应函数有 get():获取指针   release()放弃所有权但不delete 返回的是裸指针（所以一般不用）之后必须手动delete     

# 移动语义
std::move是用来转移所有权的 move后 源指针就变成了nullptr
