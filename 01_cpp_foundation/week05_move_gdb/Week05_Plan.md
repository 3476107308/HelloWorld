# Week05：GDB 调试 + C++11 类型推导 + 右值引用

## 一、本周必须掌握

### 重点 1：GDB 调试

| 知识点 | 你要能回答 |
|---|---|
| 断点操作 | `break` 怎么在函数入口、指定行号、条件触发设断点？`info breakpoints` 怎么查看所有断点？ |
| 单步执行 | `next` 和 `step` 的区别是什么？（一个不进入函数，一个进入）`finish` 干什么用？ |
| 查看变量 | `print` 和 `display` 的区别？怎么打印数组/指针指向的内容？ |
| 调用栈 | `backtrace`(bt) 怎么看？`frame` 怎么切换栈帧？段错误时调用栈能告诉你什么？ |
| coredump | 程序崩溃后怎么用 `gdb 程序 core文件` 定位崩溃位置？ |

### 重点 2：auto 类型推导

| 知识点 | 你要能回答 |
|---|---|
| auto 推导规则 | `auto`、`auto&`、`const auto&`、`auto&&` 分别推导出什么类型？什么时候会丢掉 const 和引用？ |
| auto 使用场景 | 迭代器、lambda、模板返回值——哪些场景用 auto 最合适？哪些场景不该用 auto？ |
| auto 与初始化列表 | `auto x = {1,2,3}` 推导出什么类型？为什么？ |

### 重点 3：decltype + 返回类型后置

| 知识点 | 你要能回答 |
|---|---|
| decltype 规则 | `decltype(变量)` 和 `decltype(表达式)` 结果有什么区别？`decltype((x))` 双括号有什么特殊含义？ |
| 返回类型后置 | `auto foo() -> int` 这种写法什么场景下必须用？（模板函数返回类型依赖参数时） |
| auto vs decltype | 什么时候用 auto 返回类型，什么时候必须用 decltype？ |

### 重点 4：nullptr + using 别名

| 知识点 | 你要能回答 |
|---|---|
| nullptr | `NULL` 和 `nullptr` 有什么区别？为什么函数重载时 `NULL` 可能调到 `int` 版本？ |
| using vs typedef | `using func_t = void(*)(int);` 和 `typedef void(*func_t)(int);` 哪个更易读？模板别名能用 typedef 吗？ |

### 重点 5：右值引用 + std::move + std::forward

| 知识点 | 你要能回答 |
|---|---|
| 左值和右值 | 怎么判断一个表达式是左值还是右值？右值引用 `T&&` 绑定什么？ |
| std::move | `std::move` 做了什么？它真的"移动"数据了吗？（没有，它只是 cast 成右值） |
| 移动语义 | 移动构造函数 vs 拷贝构造函数——什么情况下移动比拷贝高效？为什么？ |
| std::forward | `std::forward` 和 `std::move` 的区别？什么是完美转发？为什么需要 `T&&` + `std::forward<T>` 配合？ |
| 万能引用 | `T&&` 什么情况下是右值引用，什么情况下是万能引用（转发引用）？ |

---

## 二、推荐学习资料

### 主线教学
- Claude 生成 `example.cpp`（详细中文注释）+ `exercise.cpp`（需求骨架，独立完成）

### 调试专项
- **simplesoft GDB 调试合集**（27+集）：https://space.bilibili.com/391667478/lists/8218249
  - Day 1~5 每天看 3-5 集，看完立刻在自己的代码上实操
  - 重点：基本命令、断点技巧、coredump 分析

### 补充视频（课后加深理解）
- **大丙 C++11 新特性详解**：https://space.bilibili.com/147020887/lists/6687062
  - 「第二部分：类型推导」— auto、decltype、返回类型后置
  - 「第三部分：右值引用」— 右值引用、std::move、std::forward、完美转发

### 文档查阅
- GDB 官方文档：https://sourceware.org/gdb/documentation/
- cppreference auto：https://en.cppreference.com/w/cpp/language/auto
- cppreference decltype：https://en.cppreference.com/w/cpp/language/decltype
- cppreference 右值引用：https://en.cppreference.com/w/cpp/language/reference
- cppreference std::move：https://en.cppreference.com/w/cpp/utility/move
- cppreference std::forward：https://en.cppreference.com/w/cpp/utility/forward

---

## 三、每日安排

### Day 1：GDB 调试基础

**学习内容：**
GDB 是 C++ 程序员定位 bug 的核心工具。今天掌握断点设置、单步执行、查看变量和调用栈。用 GDB 调试第 4 周的 Array<T> 程序，练习 `break/run/next/step/print/backtrace/frame` 等基本命令。

**对应学习视频：**
- simplesoft GDB 调试合集 P1-P5（基本命令 + 断点 + 变量查看）
  - 链接：https://space.bilibili.com/391667478/lists/8218249

**代码练习：**
1. 编译 week04/day3 的 Array<T> 程序（加 `-g` 生成调试信息）
2. ① 在 `main` 函数入口设断点，`run` 启动，`next` 逐行执行
3. ② 在 `push_back` 函数设断点，`step` 进入函数内部
4. ③ 用 `print` 查看 `data_` 指针、`size_`、`cap_` 的值
5. ④ 用 `display` 设置自动显示变量
6. ⑤ 故意制造一个段错误（访问越界），用 `backtrace` 定位崩溃位置
7. ⑥ 记录每个 GDB 命令的作用

**建议文件名：** `day1/example_gdb_commands.cpp`（练习用，可以直接用 week04/day3 的代码）

**今日验收：**
- [ ] 能用 GDB 的 `break/run/next/step/print/backtrace` 调试自己的代码
- [ ] 能通过 `backtrace` 定位段错误的代码行号

---

### Day 2：auto 类型推导

**学习内容：**
`auto` 让编译器自动推导变量类型。关键要理解推导规则：`auto` 会丢掉 const 和引用，`auto&` 保留引用，`const auto&` 绑定到临时对象延长生命周期，`auto&&` 是万能引用。掌握什么场景适合用 auto（迭代器、lambda、泛型），什么场景不该用（代码可读性变差）。

**对应学习视频：**
- 大丙 C++11 新特性详解「第二部分：auto 类型推导」（合集内相关章节）
  - 合集链接：https://space.bilibili.com/147020887/lists/6687062
- simplesoft GDB 调试合集 P6-P10（继续每日 GDB 练习）

**代码练习：**
1. 写一个函数返回 `const vector<int>&`，用 `auto`、`auto&`、`const auto&`、`auto&&` 分别接收，对比推导出的类型（用 `typeid` 或编译器错误信息验证）
2. 用 `auto` 声明迭代器、lambda 变量、范围 for 循环变量
3. 用 GDB 在 auto 变量处设断点，用 `ptype` 查看 GDB 推导出的类型
4. 写一个场景：`auto` 推导出 `initializer_list` 的情况

**建议文件名：** `day2/example_auto.cpp`、`day2/exercise_auto.cpp`

**今日验收：**
- [ ] 能说出 `auto`、`auto&`、`const auto&`、`auto&&` 的推导区别
- [ ] 能用 `ptype` 在 GDB 中查看 auto 变量的实际类型

---

### Day 3：decltype + 返回类型后置

**学习内容：**
`decltype` 获取表达式的类型而不求值，关键区分 `decltype(变量)` 和 `decltype(表达式)`（前者保留顶层 const 和引用，后者看值类别）。`decltype((x))` 双括号永远推导为引用。返回类型后置 `auto foo() -> decltype(...)` 在模板函数返回类型依赖参数时必不可少。

**对应学习视频：**
- 大丙 C++11 新特性详解「第二部分：decltype + 返回类型后置」
  - 合集链接：https://space.bilibili.com/147020887/lists/6687062
- simplesoft GDB 调试合集 P11-P15

**代码练习：**
1. 写一个模板函数 `add(T a, U b)`，用 `auto + decltype(a+b)` 作为返回类型后置，让它能返回正确的类型
2. 对比 `decltype(x)`、`decltype((x))`、`decltype(std::move(x))` 的结果差异
3. 用 `decltype` 配合 `std::vector::iterator` 声明迭代器类型
4. 用 GDB 断点跟进模板函数，观察返回类型的实际推导

**建议文件名：** `day3/example_decltype.cpp`、`day3/exercise_decltype.cpp`

**今日验收：**
- [ ] 能写一个返回类型后置的模板函数
- [ ] 能解释 `decltype((x))` 为什么是引用类型

---

### Day 4：nullptr + using 别名 + 右值引用基础

**学习内容：**
`nullptr` 是 `std::nullptr_t` 类型的字面量，解决了 `NULL` 在函数重载时匹配到 `int` 的经典 bug。`using` 别名比 `typedef` 更易读，而且支持模板别名（`typedef` 做不到）。右值引用 `T&&` 绑定到临时对象/将亡值，是实现移动语义的基础——理解左值（有名字、可取地址）和右值（字面量、临时对象、`std::move` 结果）的区别。

**对应学习视频：**
- 大丙 C++11 新特性详解「nullptr/using」「第三部分：右值引用基础」
  - 合集链接：https://space.bilibili.com/147020887/lists/6687062
- simplesoft GDB 调试合集 P16-P20

**代码练习：**
1. 写两个重载函数 `f(int)` 和 `f(void*)`，分别传入 `NULL` 和 `nullptr`，观察调用哪个版本
2. 用 `using` 给 `unordered_map<string, vector<int>>` 起别名，再写一个模板别名 `template<typename T> using Vec = vector<T>`
3. 写一个函数，分别用 `X&`、`const X&`、`X&&` 三个重载版本接收参数，传左值、右值、`std::move` 结果进去，观察哪个重载被调用
4. 用 GDB 在右值引用参数的函数设断点，观察传入的地址

**建议文件名：** `day4/example_rvalue.cpp`、`day4/exercise_rvalue.cpp`

**今日验收：**
- [ ] 能解释 `NULL` vs `nullptr` 在重载解析时的区别
- [ ] 能用 `using` 写模板别名
- [ ] 能判断一个表达式是左值还是右值

---

### Day 5：std::move + std::forward 完美转发

**学习内容：**
`std::move` 本质上就是 `static_cast<T&&>`——它不移动数据，只把左值转成右值，真正的移动由移动构造函数/移动赋值完成。`std::forward` + 万能引用 `T&&` 实现完美转发：参数是左值就转发为左值引用，是右值就转发为右值引用，保留原始参数的值类别。这是 `make_shared`、`emplace_back` 等工厂函数的基础。

**对应学习视频：**
- 大丙 C++11 新特性详解「第三部分：std::move、std::forward、完美转发」
  - 合集链接：https://space.bilibili.com/147020887/lists/6687062
- simplesoft GDB 调试合集 P21-P25

**代码练习：**
1. 写一个带移动构造函数和移动赋值运算符的类（如 Buffer），用 GDB 在移动函数设断点，观察资源转移过程
2. 写一个工厂函数 `makeObject`，用 `T&&` + `std::forward<T>` 完美转发参数给构造函数
3. 对比 `std::move` 和 `std::forward`：写一个转发函数，分别用 `move` 和 `forward` 转发参数，观察区别
4. 在自己写的 `Array<T>` 模板中加一个 `emplace_back` 方法（选做）

**建议文件名：** `day5/example_move_forward.cpp`、`day5/exercise_move_forward.cpp`

**今日验收：**
- [ ] 能写出带移动构造和移动赋值的类
- [ ] 能解释 `std::move` 为什么本身不移动数据
- [ ] 能解释 `std::forward` 怎么实现"左值转发为左值，右值转发为右值"

---

### Day 6：本周验收项目 — 带调试日志的 String 类

**项目名：** `week05_project_string`

**目录结构：**
```
week05/
  project/
    string.hpp      — MyString 类声明
    string.cpp      — MyString 类实现
    main.cpp        — 测试和演示
    CMakeLists.txt  — 构建文件
    README.md       — 构建、运行说明
```

**需求清单：**

1. **类 MyString**
   - 字段：`char* data_`、`size_t size_`
   - 构造函数：`MyString(const char* s)` — 从 C 字符串构造
   - 析构函数：释放 `data_`
   - 拷贝构造 + 拷贝赋值（深拷贝）
   - **移动构造 + 移动赋值**（标记 `noexcept`，转移 `data_` 指针，源对象置空）
   - `size() const` 返回字符串长度（不含 `\0`）
   - `c_str() const` 返回 `const char*`
   - `MyString operator+(const MyString& other) const` — 拼接
   - `bool operator==(const MyString& other) const` — 比较
   - `char& operator[](size_t index)` — 下标访问

2. **调试日志**
   - 每个构造/析构/赋值函数在执行时打印一行日志：
     - 如 `[拷贝构造] 0x地址 -> 0x地址，内容: xxx`
   - 移动操作日志标出"源对象已置空"

3. **右值引用 + 完美转发（进阶）**
   - 写一个全局函数 `template<typename T> MyString make_string(T&& arg)`，用 `std::forward<T>` 转发
   - 写一个函数 `void take_string(MyString s)`（按值传参），分别传左值 MyString 和 `std::move` 的结果，观察打印日志验证是否触发了移动

4. **GDB 调试验证**
   - 编译时加 `-g -O0`
   - 在下面几个位置各设一个断点，用 GDB 运行并截图：
     - 移动构造函数入口
     - 拷贝构造函数入口
     - `take_string` 函数入口
   - 用 `backtrace` 展示调用栈

5. **CMake 构建（进阶）**
   - 写 `CMakeLists.txt`，用 `cmake -S . -B build && cmake --build build` 构建

**验收标准：**
- [ ] 所有构造/析构/赋值打印日志，日志清晰可读
- [ ] 用 GDB 逐语句调试，能定位到构造函数内部
- [ ] 移动后源对象 `data_` 为 `nullptr`
- [ ] 不内存泄漏、不重复释放
- [ ] `g++ -std=c++17 -Wall -g` 零 warning 通过
- [ ] README 写清楚编译和 GDB 调试命令

---

### Day 7：复盘 + 本周面试题

**复盘要点：**
1. 整理 GDB 命令速查表（最少 10 个命令）
2. 用笔在纸上画出 `MyString` 移动前后的内存布局变化
3. 检查 review.md：auto 推导规则、decltype 规则、move vs forward 对比表

**本周 10 个必须能回答的面试题：**

1. `auto` 推导时什么时候会丢掉 const？什么时候不会？
2. `decltype(x)` 和 `decltype((x))` 的结果有什么区别？为什么？
3. 什么是返回类型后置？什么场景下必须用它？
4. `NULL` 和 `nullptr` 有什么区别？函数重载时 `NULL` 可能出什么问题？
5. `using` 和 `typedef` 有什么区别？模板别名能用 `typedef` 吗？
6. 什么是左值？什么是右值？如何判断一个表达式是左值还是右值？
7. `std::move` 的实现原理是什么？它真的"移动"数据了吗？
8. 什么是万能引用（转发引用）？`T&&` 什么时候是右值引用，什么时候是万能引用？
9. `std::forward` 和 `std::move` 的区别是什么？完美转发解决什么问题？
10. GDB 中 `next` 和 `step` 的区别？`backtrace` 命令的作用是什么？怎么在 GDB 中设置条件断点？

---

## 四、本周不要踩的坑

1. **GDB 调试不编译加 `-g`**：没加 `-g` 符号信息，GDB 看不到源码和变量名。命令：`g++ -std=c++17 -Wall -g -O0 file.cpp`
2. **把 `std::move` 当"移动数据"**：`move` 只做类型转换（cast 成右值），真正的移动由移动构造函数完成。`move` 之后不写对应的移动构造/移动赋值，等于白写。
3. **`auto&&` 不一定是右值引用**：`auto&&` 是万能引用——右值初始化时是右值引用，左值初始化时是左值引用（引用折叠）
4. **用 `std::move` 之后还访问源对象**：移动后源对象处于"有效但未指定"状态——可以赋值、可以析构，但除了这两件事，其他操作结果不确定。
5. **`std::forward` 忘记传模板参数**：`std::forward<T>(arg)` 必须带 `<T>`，不写 `<T>` 编译器不知道怎么转发。

---

## 五、编译/环境备注

### GDB 环境
- Windows + MSYS2：在 MSYS2 终端中安装 `pacman -S mingw-w64-ucrt-x86_64-gdb`
- 或者使用 WSL2 + Ubuntu：`sudo apt install gdb`
- 验证安装：`gdb --version`

### 编译命令（本周所有代码）
```bash
# 调试模式（必须）-g
g++ -std=c++17 -Wall -g -O0 file.cpp -o file.exe

# 启动 GDB 调试
gdb ./file.exe
```

### GDB 常用命令速查
| 命令（缩写） | 作用 |
|---|---|
| `break main` (`b main`) | 在 main 函数设断点 |
| `break file.cpp:42` | 在 file.cpp 第 42 行设断点 |
| `break 42 if x > 100` | 条件断点：x>100 时暂停 |
| `run` (`r`) | 运行程序 |
| `next` (`n`) | 单步执行（不进入函数内部） |
| `step` (`s`) | 单步执行（进入函数内部） |
| `finish` | 执行完当前函数并返回 |
| `continue` (`c`) | 继续执行到下一个断点 |
| `print x` (`p x`) | 打印变量 x 的值 |
| `print *ptr` | 打印指针指向的内容 |
| `display x` | 每次暂停自动显示 x |
| `backtrace` (`bt`) | 显示调用栈 |
| `frame N` | 切换到第 N 层栈帧 |
| `ptype x` | 打印变量的类型 |
| `quit` (`q`) | 退出 GDB |
