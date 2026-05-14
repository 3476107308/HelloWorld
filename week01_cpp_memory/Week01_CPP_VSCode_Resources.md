# 第 1 周：C++ 指针/引用/内存 + VS Code 操作补充

时间：5.13-5.19

目标：
- 掌握指针、引用、`const`、`sizeof`、内存分区。
- 会使用 `new/delete`、`new[]/delete[]`，理解内存泄漏、野指针、悬垂指针。
- 从 Visual Studio 过渡到 VS Code，能完成单文件编译、运行、断点调试。

## 一、推荐视频课程

### 主线课程：黑马 C++ 入门/核心

链接：
- https://www.bilibili.com/video/BV15K4y1t78C/

建议观看顺序：
- `const` 增强。
- 引用的基本概念。
- 引用的本质。
- 常量指针。
- 引用作为函数返回值。
- 指针引用。
- `const` 引用。
- 内存分区模型。
- `new` 操作。
- 堆区数组。

学习方式：
- 不要只看，所有例子都在 VS Code 里重新敲一遍。
- 每看完一个小节，自己改 2-3 行代码验证现象。

### 补充课程：指针和动态内存专题

链接：
- https://www.bilibili.com/video/BV1Mm4y1Y76g

重点看：
- 指针的本质。
- `new/delete` 动态内存管理。
- 指针和数组。

适合用途：
- 如果黑马的指针部分听完还不稳，用这个视频补一遍。

### VS Code 配置视频

推荐 1：
- https://www.bilibili.com/video/BV19c411G7ey/

推荐 2：
- https://www.bilibili.com/video/BV1356zYMEp5/

建议：
- 你之前用 Visual Studio，第一周优先走 MSVC 路线，不要急着配置 MinGW。
- 如果视频里讲 MinGW，可以先看“VS Code 安装、插件、运行、调试”的部分；编译器部分参考微软官方 MSVC 文档。

## 二、官方文档

VS Code C/C++ 总入口：
- https://code.visualstudio.com/docs/languages/cpp

VS Code 配置 MSVC：
- https://code.visualstudio.com/docs/cpp/config-msvc

说明：
- VS Code 本身只是编辑器，不自带 C++ 编译器。
- 如果你已经装了 Visual Studio，并且安装过“使用 C++ 的桌面开发”工作负载，就可以直接复用 MSVC 编译器。

## 三、Visual Studio 转 VS Code：第一周够用操作

### 1. 安装插件

VS Code 里按 `Ctrl+Shift+X` 打开扩展，安装：
- `C/C++`，Microsoft 官方插件。
- `CMake Tools`，第 7 周开始会大量用，先装好。
- `Code Runner`，可选，适合快速跑单文件，但不要依赖它做正式项目。

### 2. 推荐打开方式

如果使用 MSVC：
1. Windows 开始菜单搜索 `Developer PowerShell for VS 2022` 或 `Developer Command Prompt for VS 2022`。
2. 在里面进入你的代码目录。
3. 输入 `code .` 打开 VS Code。

原因：
- 普通 PowerShell 可能找不到 `cl.exe`。
- Developer Prompt 会自动配置 MSVC 的环境变量。

### 3. 第一个文件

新建目录：
- `week01_cpp_memory`

新建文件：
- `main.cpp`

示例：
```cpp
#include <iostream>
using namespace std;

int main() {
    int a = 10;
    int* p = &a;

    cout << "a = " << a << endl;
    cout << "&a = " << &a << endl;
    cout << "p = " << p << endl;
    cout << "*p = " << *p << endl;

    return 0;
}
```

### 4. 手动编译运行

MSVC：
```powershell
cl /EHsc main.cpp
.\main.exe
```

MinGW：
```powershell
g++ -std=c++17 -Wall -Wextra -g main.cpp -o main.exe
.\main.exe
```

第一周建议：
- 你电脑上如果已有 Visual Studio，就先用 `cl`。
- 等到第 7 周学 CMake，再统一项目构建方式。

### 5. VS Code 调试

基本操作：
- 打断点：代码行号左边点一下。
- 开始调试：`F5`。
- 单步跳过：`F10`。
- 单步进入：`F11`。
- 继续运行：`F5`。
- 查看变量：左侧 `Variables`。
- 监视表达式：`Watch` 里添加 `p`、`*p`、`&a`。

第一周重点调试：
- 指针变量保存的地址。
- 解引用 `*p` 的值。
- `new` 后地址是否为空。
- `delete` 后不要再次访问。

## 四、第一周每天安排

### Day 1：指针基础

看：
- 黑马 C++ 指针基础相关小节。

写：
- 指针访问普通变量。
- 指针作为函数参数实现 `swap`。

验收：
- 能解释 `p`、`&p`、`*p` 的区别。

### Day 2：引用和 const

看：
- 引用基本概念。
- 引用本质。
- `const` 引用。
- 常量指针。

写：
- 值传递、指针传递、引用传递三个版本的 `swap`。
- 写出三种 `const` 指针形式。

验收：
- 能区分 `const int* p`、`int* const p`、`const int* const p`。

### Day 3：sizeof 和内存布局

看：
- 内存分区模型。
- 栈区、堆区、全局区、代码区。

写：
- 打印局部变量、全局变量、静态变量、字符串常量、堆区变量地址。
- 定义结构体，观察 `sizeof` 和对齐。

验收：
- 能解释为什么结构体大小不一定等于成员大小相加。

### Day 4：new/delete

看：
- `new` 操作。
- 堆区数组。

写：
- `new int` / `delete`。
- `new int[n]` / `delete[]`。
- 动态输入数组长度，求和、最大值、平均值。

验收：
- 不混用 `delete` 和 `delete[]`。

### Day 5：野指针、悬垂指针、内存泄漏

看：
- 指针和动态内存专题视频。

写：
- 故意写 3 个错误案例：未初始化指针、释放后继续访问、忘记释放。
- 再写修正版。

验收：
- 能说出每类错误为什么危险。

### Day 6：验收项目

写：
- `DynamicArray` 或 `IntArray`。

最低功能：
- 构造函数。
- 析构函数。
- `push_back`。
- `size`。
- `operator[]`。
- 自动扩容。

验收：
- 能插入 100 个整数。
- 能输出所有元素。
- 析构时释放内存。

### Day 7：复盘

整理：
- `week01_note.md`。

内容：
- 10 个概念解释。
- 5 个容易错的代码片段。
- 本周项目运行截图。

提交：
- GitHub 或本地 Git 至少 3 次 commit。

## 五、第一周最终作业

项目名：
- `week01_dynamic_array`

文件结构：
```text
week01_dynamic_array/
  main.cpp
  IntArray.h
  IntArray.cpp
  README.md
```

README 至少包含：
- 项目功能。
- 编译方式。
- 运行方式。
- 学到的知识点。
- 遇到的问题。
