# Week06：C++11 核心进阶与 CMake 工程化

## 本周定位

本周不是重新学习 C++，而是把已有的 C++11 知识整理成可以构建、调试和解释的多文件工程。完成后，你应该能在 VMware Ubuntu 中独立创建一个小型 C++ 项目，并用 CMake 配置和编译。

每天建议投入 2.5-3.5 小时，其中编码和调试时间不少于一半。

## 一、本周必须掌握

### 1. C++11 核心进阶

- `constexpr`：编译期常量和普通运行时变量的区别。
- `=default`、`=delete`：显式要求或禁止编译器生成特殊成员函数。
- 委托构造和继承构造：减少构造函数重复代码。
- lambda：捕获列表、参数、返回类型和可变性。
- `std::function`：统一保存普通函数、lambda 和函数对象。

你要能回答：

- `constexpr` 和 `const` 的使用目的有什么不同？
- 为什么需要用 `=delete` 禁止拷贝？
- `[=]`、`[&]`、`[this]` 的捕获区别是什么？
- `std::function` 的适用场景和可能的开销是什么？

### 2. CMake 基础

- `cmake_minimum_required`、`project`、`add_executable`。
- 多个 `.cpp` 文件参与同一个目标的编译。
- `target_include_directories` 配置头文件搜索路径。
- `target_compile_features` 或 `CMAKE_CXX_STANDARD` 指定 C++ 标准。
- `cmake -S . -B build` 配置和 `cmake --build build` 编译。
- 源码目录、构建目录和生成文件分离。

你要能回答：

- CMake 和编译器分别负责什么？
- 为什么不应该把构建文件直接放在源码目录？
- `target_include_directories` 解决了什么问题？
- 修改 `CMakeLists.txt` 后为什么需要重新配置？

### 3. 静态库与工程边界

- 头文件放声明，源文件放实现。
- 静态库的基本作用和链接方式。
- 目标之间的依赖关系。
- 用目录结构表达模块边界，而不是把所有代码写进 `main.cpp`。

## 二、推荐学习资料

### 主线

- CMake 官方教程：<https://cmake.org/cmake/help/latest/guide/tutorial/index.html>
  - 重点看：基本语法、`add_executable`、多文件工程、构建目录。
- CMake 官方命令文档：<https://cmake.org/cmake/help/latest/manual/cmake-commands.7.html>
- C++11 新特性合集：<https://space.bilibili.com/147020887/lists/6687062>
  - 重点复习：`constexpr`、`=default/=delete`、lambda、`std::function`。

原计划中的 B 站 CMake 视频目前不可用，因此不再把它列为必需资料。视频只能作为可选补充，搜索关键词为“CMake 多文件工程 CMakeLists add_library target_link_libraries”；找不到合适视频时直接使用官方教程即可。

### 文档

- CMake 官方教程：<https://cmake.org/cmake/help/latest/guide/tutorial/index.html>
- `add_executable`：<https://cmake.org/cmake/help/latest/command/add_executable.html>
- `target_include_directories`：<https://cmake.org/cmake/help/latest/command/target_include_directories.html>
- cppreference lambda：<https://en.cppreference.com/w/cpp/language/lambda>
- cppreference `std::function`：<https://en.cppreference.com/w/cpp/utility/functional/function>

不要同时打开大量课程。先按每日任务看对应章节，再立即写代码；遇到具体问题时再查官方文档。

## 三、每日安排

### Day 1：CMake 多文件工程入门

学习内容：

- 理解预处理、编译、汇编、链接的大致流程。
- 创建 `include/`、`src/` 和 `build-linux/` 目录。
- 用 CMake 编译一个包含头文件和多个源文件的小项目。

代码产出：

- `day01/example.cpp`：演示一个最小 CMake 工程需要哪些文件。
- `day01/exercise.cpp`：独立完成一个多文件“学生成绩统计”工程的需求骨架。
- `day01/review.md`：完成练习后记录构建流程和命令含义。

今日验收：

- 能在 Ubuntu 中执行 `cmake -S . -B build-linux` 和 `cmake --build build-linux`。
- `main.cpp` 不直接实现全部功能，至少调用一个来自独立 `.cpp` 文件的函数。
- 能解释源码目录和构建目录的区别。

### Day 2：`constexpr`、`=default`、`=delete`

学习内容：

- 对比编译期常量、运行时常量和普通变量。
- 为类显式默认构造、禁止拷贝或禁止赋值。
- 观察编译器报错并理解错误原因。

代码练习：

- 编写 `Config` 类，使用 `constexpr` 保存固定配置。
- 编写不可复制类 `NonCopyable`，验证拷贝构造和拷贝赋值被禁止。

今日验收：

- 能写出至少一个有效的 `constexpr` 函数。
- 能解释 `=delete` 比把函数声明为 `private` 更明确的地方。

### Day 3：lambda 与 `std::function`

学习内容：

- lambda 的捕获方式、参数、返回值和生命周期风险。
- 使用 `std::function` 保存回调。
- 让业务代码通过回调通知 `main`，减少模块耦合。

代码练习：

- 对学生成绩 `vector` 使用 lambda 完成排序和筛选。
- 写一个 `run_task(std::function<void(int)>)`，模拟任务进度回调。

今日验收：

- 能说明按值捕获和按引用捕获的区别。
- 能传入普通函数、lambda 和函数对象三种回调。

### Day 4：静态库和目标依赖

学习内容：

- 使用 `add_library(... STATIC ...)` 创建静态库。
- 使用 `target_link_libraries` 将库链接到可执行程序。
- 理解库的头文件声明、实现文件和调用方之间的关系。

代码练习：

- 将 Day 1 的成绩统计功能拆为 `score_lib` 静态库。
- 主程序只依赖公开头文件，不直接包含实现文件。

今日验收：

- 修改库实现后能重新构建并看到结果变化。
- 能解释 `target_link_libraries` 的作用。

### Day 5：GDB 与 CMake 工程调试

学习内容：

- 使用 Debug 构建保留调试符号。
- 在 Ubuntu 中用 GDB 设置断点、查看变量、查看调用栈。
- 故意制造一个空指针或越界错误，再用 GDB 定位。

代码练习：

- 给成绩统计库增加一个可复现的错误场景。
- 记录 `break`、`run`、`next`、`step`、`print`、`bt`、`continue` 的实际输出和作用。

今日验收：

- 能用 GDB 定位到错误发生的函数和代码行。
- 能说明为什么调试构建不能使用过高优化级别。

### Day 6：本周验收项目

项目名：`student_score_cli_cmake`

目录结构：

```text
week_project/
├── include/
│   ├── student.h
│   └── score_service.h
├── src/
│   ├── main.cpp
│   ├── student.cpp
│   └── score_service.cpp
├── tests/
│   └── score_service_test.cpp
├── CMakeLists.txt
├── README.md
└── build-linux/       # 本地生成，不提交 Git
```

需求清单：

1. `Student` 数据结构
   - 字段：`std::string id`、`std::string name`、`int math`、`int cpp`、`int english`。
   - 成员函数：`int total() const`，返回三科总分。
   - 成员函数：`double average() const`，返回平均分。

2. `ScoreService` 类
   - 字段：`std::vector<Student> students_`。
   - `void add_student(const Student& student)`：添加学生。
   - `std::vector<Student> sort_by_total_desc() const`：按总分降序返回副本。
   - `std::vector<Student> filter_average_at_least(double min_average) const`：返回平均分不低于阈值的学生。
   - `std::function<void(const Student&)> for_each_student(...) const`：使用回调遍历学生，或用等价的回调接口完成相同功能。

3. CMake 要求
   - 使用 C++17。
   - `src/` 和 `include/` 分离。
   - 至少创建一个静态库目标，再链接到可执行程序。
   - 支持 `cmake -S . -B build-linux` 和 `cmake --build build-linux`。

4. 测试要求
   - 至少测试空列表、单个学生、重复学号提示或处理、边界成绩和排序结果。
   - README 写清楚 Ubuntu 构建、运行和 GDB 命令。

验收标准：

- 能在 VMware Ubuntu 中从干净的 `build-linux/` 目录构建成功。
- `g++ -Wall -Wextra` 不产生警告。
- 业务代码不全部堆在 `main.cpp`。
- 至少有 3 个可复现的测试场景。
- 能画出 `main -> score_service -> student` 的依赖关系。

### Day 7：复盘和面试题

完成：

- `week_note.md`：总结本周学会、卡住、修复和下周调整。
- 整理 10 个面试题并写出自己的答案。
- 检查 README、构建命令和 Git 提交记录。

本周必须能回答：

1. CMake、编译器和链接器分别负责什么？
2. `cmake -S . -B build` 做了什么？
3. 为什么构建目录应该和源码目录分离？
4. `add_executable` 和 `add_library` 的区别是什么？
5. `target_include_directories` 解决什么问题？
6. `target_link_libraries` 解决什么问题？
7. 静态库和动态库的基本区别是什么？
8. `constexpr` 和 `const` 有什么区别？
9. lambda 按值捕获和按引用捕获有什么风险？
10. 为什么 GDB 调试通常要使用 `-g -O0`？

## 四、本周不要踩的坑

1. 不要把 `build-linux/` 提交到 Git；它是 CMake 生成的本地目录。
2. 不要在 CMake 中只写 `main.cpp`，却忘记把其他源文件加入目标。
3. 不要通过 `#include "xxx.cpp"` 解决链接错误；应修正 `add_executable` 或库链接配置。
4. 不要在 Windows 生成的构建目录中直接使用 Ubuntu 编译；两个环境使用不同构建目录。
5. 不要只看 CMake 视频；Day 1 必须在 Ubuntu 中实际配置、构建并运行项目。

## 五、环境命令

在 Ubuntu 项目根目录执行：

```bash
rm -rf build-linux
cmake -S . -B build-linux -DCMAKE_BUILD_TYPE=Debug
cmake --build build-linux -j2
./build-linux/<可执行文件名>
gdb ./build-linux/<可执行文件名>
```

如果系统没有工具：

```bash
sudo apt update
sudo apt install build-essential cmake gdb
```

## 六、开始顺序

1. 先阅读本文件的 Day 1。
2. 在 `01_cpp_foundation/week06_cmake/day01/` 创建当天代码目录。
3. 优先在 VMware Ubuntu 中完成构建；Windows 只作为补充验证环境。
4. 完成后把 `example.cpp`、`exercise.cpp`、`CMakeLists.txt`、构建输出和问题发来检查。
