# 第 4 周学习计划：模板、智能指针、RAII

时间：6.10-6.16
对应总计划：第 4 周，模板、智能指针、RAII
本周目标：能独立写模板类和模板函数，理解智能指针的所有权和 RAII 思想，不再手动 delete 多态对象。

---

## 一、本周必须掌握

### 1. 函数模板

重点：
- 模板的语法：`template<typename T>` / `template<class T>`
- 函数模板 vs 普通函数：模板是"蓝图"，调用时才生成具体函数
- 自动类型推导：`max(3, 5)` → `T = int`
- 显式指定类型：`max<double>(3, 5.0)`

你要能回答：
- 函数模板和普通函数有什么区别？
- `template<typename T>` 里的 `T` 是什么时候确定的？
- 什么时候必须显式指定模板参数？

### 2. 类模板

重点：
- 类模板的语法：`template<typename T> class Array { ... };`
- 类模板的成员函数类外实现：每一处都要写 `template<typename T>`
- 类模板分文件编写：模板的声明和实现**必须**放在同一个头文件中（不能分离到 .cpp，链接时会找不到）
- 类模板做函数参数：三种指定方式

你要能回答：
- 为什么类模板不能像普通类一样 .h 声明 + .cpp 实现？
- 类模板的成员函数是什么时候编译的？
- 模板类和普通类比有什么优势？

### 3. 智能指针

重点：
- `unique_ptr`：独占所有权，不能复制，只能移动。
- `shared_ptr`：共享所有权，引用计数，最后一个指向对象的 `shared_ptr` 析构时释放资源。
- `weak_ptr`：不增加引用计数，配合 `shared_ptr` 打破循环引用。
- `make_unique` / `make_shared` 代替 `new`。

你要能回答：
- `unique_ptr` 为什么不能复制？
- `shared_ptr` 的引用计数是什么意思？什么时候对象会被释放？
- `make_unique` 比 `new` 好在哪？

### 4. RAII

重点：
- RAII = Resource Acquisition Is Initialization：资源获取即初始化。
- 核心思想：**把资源的生命周期绑定到对象的生命周期上**。
- 构造函数获取资源，析构函数释放资源。
- `unique_ptr`、`shared_ptr`、`lock_guard`、`fstream` 都是 RAII 的体现。

你要能回答：
- 什么是 RAII？
- 为什么 RAII 能防止资源泄漏？
- 除了智能指针，还能举出 RAII 的例子吗？

---

## 二、推荐学习资料

### 主线视频

黑马程序员《C++教程从0到1入门编程》：
主链接：https://www.bilibili.com/video/BV1et411b73Z/

| 章节 | 内容 | 链接 | P 数 |
|------|------|------|------|
| 函数模板 | 基本语法/注意事项/案例/与普通函数区别 | https://www.bilibili.com/video/BV1et411b73Z/?p=163 | P163-P166 |
| 类模板 | 基本语法/继承/成员函数类外实现/分文件编写/友元/数组封装案例 | https://www.bilibili.com/video/BV1et411b73Z/?p=167 | P167-P175 |

> 注意：黑马视频对 `unique_ptr` / `shared_ptr` / `RAII` 讲解较少，这部分主要靠下面补充资料。

### 补充资料

智能指针和 RAII：
- Cherno C++ 智能指针：https://www.bilibili.com/video/BV1Vt411J7uN/ （搜索 "Cherno smart pointer"）
- cppreference：
  - `std::unique_ptr`：https://en.cppreference.com/w/cpp/memory/unique_ptr
  - `std::shared_ptr`：https://en.cppreference.com/w/cpp/memory/shared_ptr
  - `std::weak_ptr`：https://en.cppreference.com/w/cpp/memory/weak_ptr
  - RAII：https://en.cppreference.com/w/cpp/language/raii
- CSDN 搜索："C++ RAII 机制详解"

### 文档查阅

- cppreference：`std::unique_ptr`、`std::shared_ptr`、`std::weak_ptr`、RAII
- 《C++ Primer》第 16 章（模板）

---

## 三、每日安排

### Day 1：函数模板

学习内容：
- 模板的概念：把类型变成参数
- 函数模板的语法、自动类型推导、显式指定类型
- 函数模板的局限性（需要重载某些类型）

对应学习视频：
- 函数模板 P163-P166：https://www.bilibili.com/video/BV1et411b73Z/?p=163

代码练习：
- 写一个 `myMax(T a, T b)` 函数模板，测试 `int`、`double`、`string`
- 写一个 `mySwap(T& a, T& b)` 函数模板，交换两个变量的值
- 写一个排序函数模板 `mySort(vector<T>& v)`，能对 `int` 和 `string` 分别排序
- 测试显式指定类型 vs 自动推导的区别

文件：
```text
week04/
  day1/
    example.cpp
    exercise.cpp
```

今日验收：
- 能说出函数模板和普通函数的区别
- 能写出至少 3 个不同场景的函数模板

### Day 2：类模板

学习内容：
- 类模板的语法
- 类模板成员函数类外实现
- 类模板做函数参数

对应学习视频：
- 类模板 P167-P170：https://www.bilibili.com/video/BV1et411b73Z/?p=167

代码练习：
- 写一个 `Pair<K, V>` 类模板，存两个值（key 和 value），提供 getKey() 和 getValue()
- 写一个 `Container<T>` 类模板，内部用 `vector<T>` 存数据，提供 add()、get()、size()
- 类模板做函数参数：写一个函数 `printContainer`，接收 `Container<T>` 并打印所有元素

文件：
```text
week04/
  day2/
    example.cpp
    exercise.cpp
```

今日验收：
- 能解释类模板和普通类编译方式的区别
- 能写出类外实现的类模板成员函数

### Day 3：模板分文件编写 + 数组封装案例

学习内容：
- 为什么类模板不能分离 .h 和 .cpp（编译时机问题）
- 类模板与继承
- 黑马数组类封装案例

对应学习视频：
- 类模板 P171-P175：https://www.bilibili.com/video/BV1et411b73Z/?p=171

代码练习：
- 写一个 `Array<T>` 类模板，功能类似第 1 周的 IntArray，但支持任意类型：
  - 构造 / 析构
  - 拷贝构造 / 拷贝赋值（深拷贝）
  - 移动构造 / 移动赋值
  - `operator[]` 支持下标访问
  - `size()` 返回元素个数

文件：
```text
week04/
  day3/
    example.cpp
    exercise.cpp
```

今日验收：
- 能解释为什么模板不能分文件编译
- `Array<int>` 和 `Array<string>` 都能正常工作

### Day 4：unique_ptr 和 RAII

学习内容：
- RAII 的核心思想（构造获取，析构释放）
- `unique_ptr` 的基本使用
- 为什么 `unique_ptr` 不能复制，只能移动

对应学习视频：
- Cherno C++ 智能指针（搜索 "Cherno smart pointer B站"）
- cppreference `std::unique_ptr`

代码练习：
- 写一个 RAII 文件管理类 `FileGuard`（构造打开文件，析构自动关闭）
- 用 `unique_ptr<Animal>` 重写第 2 周的多态动物程序，不再手动 `delete`
- 把 `vector<unique_ptr<Animal>>` 作为参数传给函数（注意：必须用引用）
- 测试 `unique_ptr` 的移动语义：`auto p2 = std::move(p1);`

文件：
```text
week04/
  day4/
    example.cpp
    exercise.cpp
```

今日验收：
- 能用自己的话解释 RAII
- 能用 `unique_ptr` 管理动态对象，不再写 `delete`

### Day 5：shared_ptr 和 weak_ptr

学习内容：
- `shared_ptr` 的引用计数机制
- `make_shared` 用法
- `weak_ptr` 打破循环引用

对应学习视频：
- Cherno C++ 智能指针续集
- cppreference `std::shared_ptr`、`std::weak_ptr`

代码练习：
- 用 `shared_ptr` 管理一个对象，观察引用计数变化（`use_count()`）
- 写一个双向链表节点用 `shared_ptr` 和 `weak_ptr` 打破循环引用
- 对比 `unique_ptr` 和 `shared_ptr` 的适用场景

文件：
```text
week04/
  day5/
    example.cpp
    exercise.cpp
```

今日验收：
- 能解释 `shared_ptr` 的引用计数什么时候加，什么时候减
- 能说出为什么需要 `weak_ptr`

### Day 6：本周验收项目 —— 模板化学生管理系统 + 智能指针重构

项目名：
```text
week04/finish/
```

#### 需求清单

**需求 1：把第 3 周的 StudentRepository 改成类模板**

要求：
- `template<typename T> class Repository { ... };`
- `T` 可以是 `Student`，也可以是任何有 `id()` 方法的类型
- Repository 提供：add、remove、update、find_by_id、list_all、sort（用 lambda 自定义比较规则）、count
- 内部仍用 `vector<T>` + `unordered_map<string, int>` 做索引

**需求 2：使用 unique_ptr 管理 Student 对象**

要求：
- 验收项目 main.cpp 中不使用裸指针 `new` / `delete`
- 所有需要多态或动态分配的地方用 `unique_ptr`

**需求 3：Array<T> 模板化**

要求：
- 把 day3 的 `Array<T>` 独立出来，作为 `Array.h`，在本周验收项目中使用
- 用 `Array<Student>` 替代部分 `vector<Student>` 的场景，验证模板类的复用性

**需求 4：使用 shared_ptr 共享配置对象**

要求：
- 创建一个 `Config` 类（存学校名称、学期等全局信息）
- 多处引用同一个 `Config` 对象，用 `shared_ptr<Config>` 传递
- 打印 `use_count()` 观察引用计数

#### 验收标准

- `Repository<T>` 能对 `Student` 类型正常工作（增删改查排序）
- 代码中不出现裸 `new` / `delete`
- 能解释 `Array<T>` 和 `vector<T>` 的结构差异
- 能演示 `shared_ptr` 引用计数的变化
- 至少 3 次 Git commit

#### 文件结构

```
week04/finish/
  main.cpp
  Student.h
  Array.h             （类模板，声明+实现都在头文件）
  Repository.h        （类模板，声明+实现都在头文件）
  Config.h
  README.md
```

### Day 7：复盘和面试题

整理文件：
```text
week04/
  week04_note.md
```

必须用自己的话写清楚：
1. 函数模板和类模板的区别？
2. 为什么类模板不能分文件编写（.h + .cpp）？
3. `typename` 和 `class` 在模板参数中有什么区别？
4. RAII 是什么？举出 3 个 RAII 的例子。
5. `unique_ptr` 为什么不能复制？
6. `shared_ptr` 的引用计数是怎么工作的？
7. `weak_ptr` 用来解决什么问题？
8. `make_unique` / `make_shared` 比直接 `new` 好在哪？
9. 什么时候用 `unique_ptr`，什么时候用 `shared_ptr`？
10. 模板的编译时机和普通函数有什么不同？

---

## 四、本周不要踩的坑

- 不要尝试把类模板的实现放在 `.cpp` 文件中（除非你懂显式实例化，否则链接必报错）
- 不要把 `unique_ptr` 当普通变量传值给函数（拷贝会编译报错），必须传引用或 `std::move`
- 不要用 `shared_ptr` 管理 `this` 时盲目传裸 `this` 指针（用 `enable_shared_from_this`）
- 不要以为智能指针 = 万能，循环引用会导致 `shared_ptr` 永远不释放
- 不要在不需要共享所有权的地方用 `shared_ptr`（优先用 `unique_ptr`）
- 模板代码不要写得太复杂——本周重点是把第 3 周的代码"模板化"，而不是发明新功能

---

## 五、编译备注

```bash
g++ -std=c++17 main.cpp -o main.exe
```

注意：如果模板全部放在头文件中（`.h`），只需要编译 `main.cpp`，不需要链接额外的 `.cpp`。
