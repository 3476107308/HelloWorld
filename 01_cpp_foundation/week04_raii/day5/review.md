## unique_ptr shared_ptr weak_ptr
# unique_ptr
是独占使用权 不能复制 只能移动 适用于一种资源只能被一个对象使用的情况
# shared_ptr
可以共享 可以复制 可以移动 适用于一种资源可以被多个对象使用的情况
# weak_ptr
是一个观察者 不增加引用计数 可以通过lock（）将权限升级为shared_ptr 防止循环引用时(双向链表 图结构 观察者模式) 内存泄漏的情况  需要访问但不拥有

# 为什么使用make_shared 和 make_unique 而不使用new
```cpp
// 用 new：两次堆分配
//   new X — 分配对象内存
//   shared_ptr 内部再分配"控制块"（存引用计数的结构）
  shared_ptr<X> p1(new X);

// 用 make_shared：一次堆分配
// 把对象和控制块放在同一块连续内存里，一次分配搞定
  auto p2 = make_shared<X>();
```

不使用new 还能防止出现异常

# 引用计数 use_count()
shared_ptr 拷贝时 计数+1
shared_ptr 销毁时 计数-1

# 循环引用
引用计数永远到不了0 所以无法析构
离开作用域 → 外部 shared_ptr 销毁，计数各减 1但 p1 内部还有个 shared_ptr 指向 p2 → p2 计数还不为 0 → p2 不析构 → p2 内部指向 p1 的 shared_ptr 也不释放 → p1 计数也不为 0 → p1 也不析构→ 两者都卡住，内存泄漏

# reset()的作用
使指针变空 引用计数-1