# Qt/C++ 实习 35 周学习计划（整合版）

适用目标：大一基础为 C++ 到面向对象、Linux 基础命令、数据结构，目标为 2027 年 1 月中旬前后争取 Qt/C++ 相关实习。

## 整合说明

本计划在原 Plan.md 基础上整合了两个优质 B 站资源：

| 博主 | 合集 | 用途 |
|------|------|------|
| 爱编程的大丙 | C++11 新特性详解（106集）、Linux 基础、CMake、多线程、网络、Qt5、线程池等 | 视频教学主线（替代黑马/千锋/尚硅谷） |
| simplesoft | GDB 调试（27+集） | 第 5 周调试专项 |

**教学方式不变：**
- `example.cpp`（Claude 生成，详细中文注释）+ `exercise.cpp`（需求骨架，独立完成）
- 大丙 video 作为**课后加深理解的参考资料**，链接标注在每周后面
- `prompt.md` 中的 review.md 规范、代码风格等约定继续沿用

## 核心原则

- 每周至少 5 天学习，每天 2.5-4 小时；周末做验收项目和复盘
- C++ 不只看语法，要写可编译、可调试、可解释的代码
- Qt 学习从第 12 周开始，但 CMake、Linux、网络要提前铺垫
- 每月必须有可展示产物：小程序、README、截图、构建说明

## 固定周节奏

- 周一到周三：example.cpp 教学 + 跟写代码
- 周四：独立完成 exercise.cpp
- 周五：整理 review.md + 回答 10 个面试式问题，追加到 `Interview_Questions.md`
- 周六：完成验收任务，提交 Git
- 周日：复盘、补 bug、写 README 或学习总结

---

## 第 1 阶段：C++ 强化（第 1-6 周）

### 第 1 周：指针、引用、const、内存

学习内容：
- 指针和引用的区别，空指针、野指针、悬垂指针
- `const int*`、`int* const`、`const int* const`
- `sizeof`、结构体对齐、栈/堆/全局区/代码区
- `new/delete`、`new[]/delete[]`，内存泄漏排查

教学方式：
- Claude 生成 `example.cpp` + `exercise.cpp`（主线）
- 大丙视频作为补充：C++11 新特性详解「第一部分：基础特性」中相关章节

练习：
- 手写动态数组类 `IntArray`，支持构造、析构、拷贝、下标访问
- 画出一个对象在内存中的生命周期

验收：
- 代码能通过 `g++ -Wall -Wextra` 编译
- 无越界、无重复释放、无明显泄漏
- 整理本周 10 个面试题，追加到 `Interview_Questions.md`

### 第 2 周：继承、多态、虚函数

学习内容：
- `public/protected/private` 继承和成员访问
- 虚函数、纯虚函数、抽象类、虚析构函数
- 静态绑定和动态绑定，虚函数表基本概念

教学方式：
- Claude 生成 `example.cpp` + `exercise.cpp`（主线）
- 大丙补充：C++11 新特性详解「final/override」相关

练习：
- `Animal` 抽象基类，派生 `Cat`、`Dog`、`Bird`
- 用 `vector<Animal*>` 或 `vector<unique_ptr<Animal>>` 多态调用

验收：
- 能解释为什么基类析构函数通常要写成 `virtual`
- 整理本周 10 个面试题，追加到 `Interview_Questions.md`

### 第 3 周：STL 基础

学习内容：
- `vector`、`string`、`map`、`unordered_map`
- 迭代器、范围 `for`、`sort/find/for_each/count_if`
- 值语义、拷贝、引用返回的风险

教学方式：
- Claude 生成 `example.cpp` + `exercise.cpp`（主线）

练习：
- 学生信息管理 CLI：增、删、改、查、按成绩排序、按学号查找

验收：
- 数据结构使用合理，查学号用 `unordered_map`
- README 写清楚编译和运行方式
- 整理本周 10 个面试题，追加到 `Interview_Questions.md`

### 第 4 周：模板、智能指针、RAII

学习内容：
- 函数模板、类模板、模板实例化
- `unique_ptr`、`shared_ptr`、`weak_ptr` 的使用场景
- RAII：资源获取即初始化

教学方式：
- Claude 生成 `example.cpp` + `exercise.cpp`（主线）
- 大丙补充：C++11 新特性详解「第四部分：智能指针」（9集）

练习：
- 写模板类 `Array<T>`（含 Rule of Five）
- 把第 2 周的动物多态改成 `vector<unique_ptr<Animal>>`

验收：
- 不再手动 `delete` 多态对象
- 能说清楚 `unique_ptr` 为什么不能复制、只能移动
- 能解释 `shared_ptr` 引用计数机制和 `weak_ptr` 用途
- 整理本周 10 个面试题，追加到 `Interview_Questions.md`

### 第 5 周：GDB 调试 + C++11 类型推导 + 右值引用

学习内容：
- GDB 调试：断点、单步、查看变量、调用栈、coredump 分析
- C++11：`auto`、`decltype`、返回类型后置、`nullptr`、`using` 别名
- 右值引用、`std::move`、`std::forward` 完美转发

教学方式：
- Claude 生成 `example.cpp` + `exercise.cpp`（C++11 语法部分）
- **simplesoft GDB 调试合集（27+集）** 作为调试专项
  - 链接：https://space.bilibili.com/391667478/lists/8218249
  - 每天看 3-5 集 + 在代码中实操
- 大丙补充：C++11 新特性详解「第二部分：类型推导」「第三部分：右值引用/move/forward」

练习：
- 用 GDB 调试第 4 周的 Array<T> 程序，练习断点、查看 data_ 指针
- 写一个带 move 语义的类，用 GDB 观察资源转移过程
- 写 lambda 表达式、auto 推导的各种场景

验收：
- 能用 GDB 定位空指针错误和内存泄漏
- 能解释左值、右值、`std::move` 的本质
- 整理本周 10 个面试题，追加到 `Interview_Questions.md`

### 第 6 周：C++11 核心进阶 + CMake 入门

学习内容：
- `constexpr`、`=default/=delete`、委托/继承构造函数
- `lambda` 表达式、可调用对象包装器 `std::function`
- CMake 基础：`add_executable`、`target_include_directories`、`target_link_libraries`

教学方式：
- Claude 生成 `example.cpp` + `exercise.cpp`（C++11 语法）
- 大丙视频：
  - C++11 新特性详解「第三部分：constexpr/using/委托构造/lambda」
  - CMake 视频链接可能失效；以 CMake 官方教程为主：https://cmake.org/cmake/help/latest/guide/tutorial/index.html

练习：
- 把第 3 周的学生管理系统用 CMake 构建
- 写 lambda 比较函数、`std::function` 回调示例

验收：
- 项目支持 `cmake -S . -B build && cmake --build build`
- 整理本周 10 个面试题，追加到 `Interview_Questions.md`

---

## 第 2 阶段：Linux、构建工具、网络（第 7-11 周）

### 第 7 周：Linux 基础命令

学习内容：
- 文件/目录操作、权限、用户/组、软硬链接
- `grep/find/tar/chmod/chown/ln/top/ps/netstat`
- 输入输出重定向、管道、vim 基本操作、gcc 编译流程

教学方式：
- **大丙：Linux 基础入门教程**（8.5h）作为主线
  - 链接：https://www.bilibili.com/video/BV1x5411t7Tj/
  - 覆盖：命令/vim/gcc/gdb/动态库静态库/makefile

练习：
- 写一组命令：查日志、打包项目、修改权限、建立软链接
- vim 编辑 C++ 代码、gcc 手动编译

验收：
- 整理 `linux_commands.md`，每个命令有一个真实例子
- 整理本周 10 个面试题，追加到 `Interview_Questions.md`

### 第 8 周：Linux 编程 + 多线程基础

学习内容：
- 静态库/动态库制作和使用
- Makefile 基础
- `fork/exec/wait/exit`、进程、信号

教学方式：
- 大丙：Linux 基础入门教程（库和 makefile 部分）
- Claude 生成 `example.cpp` + `exercise.cpp`（进程/信号部分）

练习：
- 制作静态库和动态库，分别链接
- C++ 写父进程创建子进程、捕获 Ctrl+C 优雅退出

验收：
- 能画出父子进程执行流程
- 整理本周 10 个面试题，追加到 `Interview_Questions.md`

### 第 9 周：网络基础

学习内容：
- OSI 七层、TCP/IP 四层、MAC、IP、子网掩码、网关
- TCP 三次握手、四次挥手、滑动窗口、拥塞控制
- UDP 特点和适用场景

教学方式：
- 湖科大教书匠：计算机网络
- 小林 Coding：TCP/UDP 面试题

练习：
- 手画 TCP 连接建立和断开流程
- 用 `ping/traceroute/ipconfig` 分析网络环境

验收：
- 能讲清楚同网段和跨网段通信的区别
- 整理本周 10 个面试题，追加到 `Interview_Questions.md`

### 第 10 周：Socket 编程

学习内容：
- IP、端口、字节序
- `socket/bind/listen/accept/connect/send/recv`
- 阻塞 IO 基本概念

教学方式：
- **大丙：并发网络通信 — 套接字通信**（3h）作为主线
  - 链接：https://www.bilibili.com/video/BV1We4y197jP/

练习：
- C++ 实现 TCP echo server 和 client

验收：
- 服务端可接收客户端消息并回显
- README 包含运行截图和端口说明
- 整理本周 10 个面试题，追加到 `Interview_Questions.md`

### 第 11 周：多线程 + IO 多路复用

学习内容：
- `std::thread`、`std::mutex`、`std::lock_guard`、`std::condition_variable`
- `select/poll/epoll` 概念
- HTTP 协议基础

教学方式：
- **大丙：多线程和线程同步**（3h）https://www.bilibili.com/video/BV1Yr4y1U7kR/
- **大丙：IO 多路复用**（1.5h）https://www.bilibili.com/video/BV1XG411X7LX/
- 大丙 C++11 详解「第六部分：并发与多线程」相关章节

练习：
- 用多线程实现简单的并发 echo server

验收：
- 能解释 `epoll` 和 `select` 的区别
- 整理本周 10 个面试题，追加到 `Interview_Questions.md`

---

## 第 3 阶段：Qt 入门到核心（第 12-16 周）

> **Qt5 vs Qt6 说明：** 大丙的视频基于 Qt5，但 Qt5 和 Qt6 的核心 API 95% 一致（信号槽、QWidget、布局、事件系统等）。编译方面，大丙有 CMake 教程，用 CMake 构建 Qt5/Qt6 项目写法几乎相同。大丙视频中的 qmake 用法直接替换为 CMake 即可。

### 第 12 周：Qt 环境、工程结构、基础控件

学习内容：
- 安装 Qt 6 + Qt Creator（或 VS Code + CMake + Qt）
- Qt Widgets 工程结构、CMake 配置
- `QWidget/QPushButton/QLabel/QLineEdit`
- `QHBoxLayout/QVBoxLayout/QGridLayout`

教学方式：
- **大丙：Qt 开发入门基础教程 Qt5**（9.5h）开始
  - 链接：https://www.bilibili.com/video/BV1t44y1q7jC/
- Claude 生成 `example.cpp` + `exercise.cpp`

练习：
- 登录界面：用户名、密码、登录按钮、提示文本

验收：
- 界面布局不乱，窗口缩放后控件仍合理
- 项目使用 CMake
- 整理本周 10 个面试题，追加到 `Interview_Questions.md`
- 创建初版简历框架 `resume_template.md`

### 第 13 周：信号槽和元对象系统

学习内容：
- 内置信号槽、自定义信号槽
- `Q_OBJECT`、moc、元对象系统
- 事件循环基本概念、lambda 连接写法
- **大丙：Qt 事件** 合集

教学方式：
- 大丙 Qt5 教程 + 大丙 Qt 事件合集
- Claude 生成 `example.cpp` + `exercise.cpp`

练习：
- 自定义 `LoginWidget`，登录成功发出 `loginSuccess(QString user)` 信号

验收：
- 能解释 `Q_OBJECT` 少了会出现什么问题
- 会使用新式 `connect(sender, &Sender::signal, receiver, &Receiver::slot)`
- 整理本周 10 个面试题，追加到 `Interview_Questions.md`

### 第 14 周：Qt 容器、控件、对话框

学习内容：
- `QString/QList/QMap`
- `QFile/QDir/QTextStream/QDataStream`
- `QDialog/QMessageBox/QFileDialog`
- **大丙：Qt 开发进阶 — 按钮使用**（2h）
- **大丙：Qt 开发进阶 — 容器控件**（2h）

教学方式：
- 大丙 Qt5 教程 + 大丙控件合集
- Claude 生成 `example.cpp` + `exercise.cpp`

练习：
- 文本编辑器：新建、打开、保存、另存为、状态提示

验收：
- 能正确处理文件打开失败、保存失败
- 整理本周 10 个面试题，追加到 `Interview_Questions.md`

### 第 15 周：Qt 多线程

学习内容：
- `QThread` 基本用法、`moveToThread` 推荐模式
- `QMutex/QWaitCondition`
- UI 线程不能被阻塞
- **大丙：Qt 多线程 — 线程池的使用**（1.5h）

教学方式：
- 大丙 Qt 多线程合集
- Claude 生成 `example.cpp` + `exercise.cpp`

练习：
- 后台任务模拟下载，进度条实时更新，UI 不假死

验收：
- 不在子线程直接操作 UI
- 线程结束后资源释放正确
- 整理本周 10 个面试题，追加到 `Interview_Questions.md`

### 第 16 周：Qt 数据库 + JSON

学习内容：
- `QSqlDatabase/QSqlQuery/QSqlTableModel`、SQLite
- `QJsonDocument/QJsonObject/QJsonArray`
- **大丙：Qt 编程之数据库操作**
- **大丙：Json 及 Json 在 Qt 中的使用**

教学方式：
- 大丙数据库 + Json 合集
- Claude 生成 `example.cpp` + `exercise.cpp`

练习：
- 给聊天工具或编辑器增加 SQLite 存储功能、JSON 配置读写

验收：
- 数据可增删查改，程序重启后仍保留
- 整理本周 10 个面试题，追加到 `Interview_Questions.md`

---

## 第 4 阶段：项目实战（第 17-29 周）

### 第 17 周：聊天工具项目设计

学习内容：
- `QTcpServer/QTcpSocket`
- 消息协议设计：登录、群聊、私聊、退出
- UI 草图和类设计
- **大丙：Qt 多线程网络通信（socket）**（2h）

产出：
- 项目仓库、README、功能列表、类图

验收：
- 服务端能接受一个客户端连接
- 整理本周 10 个面试题，追加到 `Interview_Questions.md`
- 创建项目说明书和演示脚本模板

### 第 18 周：聊天工具基础通信

学习内容：
- TCP 粘包/半包、用 JSON 或长度头设计消息

练习：
- 客户端登录、服务端广播上线消息

验收：
- 两个客户端能看到彼此上线和群聊消息
- 整理本周 10 个面试题，追加到 `Interview_Questions.md`

### 第 19 周：多人聊天和私聊

练习：
- 群聊、私聊、用户列表刷新
- 信号槽解耦网络层和 UI 层

验收：
- 至少 3 个客户端稳定聊天
- 整理本周 10 个面试题，追加到 `Interview_Questions.md`

### 第 20 周：聊天工具收尾

练习：
- 日志、异常处理、断线重连
- 聊天记录保存、README 项目包装

验收：
- GitHub 展示：截图、功能、技术点、构建方式
- 整理本周 10 个面试题，追加到 `Interview_Questions.md`

### 第 21 周：文件管理器项目设计

学习内容：
- `QFileSystemModel/QTreeView/QTableView`
- 文件路径、权限、大小、修改时间

产出：
- 文件浏览界面，可切换目录

验收：
- 能显示目录树和文件列表
- 整理本周 10 个面试题，追加到 `Interview_Questions.md`

### 第 22 周：文件操作

练习：
- 复制、删除、重命名
- 错误提示和确认对话框

验收：
- 对不存在路径、权限不足、同名文件有提示
- 整理本周 10 个面试题，追加到 `Interview_Questions.md`

### 第 23 周：调用系统命令

学习内容：
- `QProcess`、调用 Shell 脚本、权限修改

练习：
- 调用系统命令修改权限、执行自定义脚本并显示输出

验收：
- 不拼接危险命令
- 整理本周 10 个面试题，追加到 `Interview_Questions.md`

### 第 24 周：文件管理器收尾

练习：
- UI 整理、代码分层、搜索、收藏目录、README

验收：
- 项目可演示 5 分钟，有截图和构建说明
- 整理本周 10 个面试题，追加到 `Interview_Questions.md`

### 第 25 周：Qt HTTP 客户端

学习内容：
- `QNetworkAccessManager/QNetworkReply`
- 异步请求和错误处理

练习：
- 天气查询工具或 GitHub 用户信息查询

验收：
- 能解析 JSON 并展示关键字段
- 整理本周 10 个面试题，追加到 `Interview_Questions.md`

### 第 26 周：图形视图

学习内容：
- `QGraphicsView/QGraphicsScene/QGraphicsItem`

练习：
- 简易网络拓扑/文件关系可视化小模块

验收：
- 能拖动节点，显示节点信息
- 整理本周 10 个面试题，追加到 `Interview_Questions.md`

### 第 27 周：项目重构

学习内容：
- MVC/MVVM、业务层/UI层/网络层分离

练习：
- 重构两个核心项目的类结构

验收：
- 每个项目至少整理 3 个明确模块
- 整理本周 10 个面试题，追加到 `Interview_Questions.md`

### 第 28 周：项目打磨

练习：
- 日志、配置、用户体验细节、Release 构建、打包
- **大丙：Qt 程序的发布和打包**

验收：
- 项目能在新目录重新构建运行
- 整理本周 10 个面试题，追加到 `Interview_Questions.md`

### 第 29 周：作品集整理

练习：
- GitHub README、录制 1-2 分钟演示 GIF
- 项目讲解结构：背景、功能、技术、难点

验收：
- 两个主项目和一个小工具都能放简历
- 整理本周 10 个面试题，追加到 `Interview_Questions.md`

---

## 第 5 阶段：面试和投递（第 30-35 周）

### 第 30 周：C++ 面试专题

学习内容：
- 指针/引用/const/static、虚函数/多态/虚析构
- STL、迭代器失效、智能指针、RAII、模板

练习：
- 牛客 C++ 基础题、手写 30 个 C++ 高频问答

验收：
- 能完整讲出一个对象从构造到析构的过程
- 复习 `Interview_Questions.md` 中第 1-6 周的 C++ 面试题

### 第 31 周：Linux 和网络面试专题

练习：
- 每天 10 道网络/Linux 问答、复盘 socket 项目

验收：
- 能手画 TCP、HTTP、DNS 流程图
- 复习第 7-11 周面试题

### 第 32 周：Qt 面试专题

练习：
- 信号槽、元对象系统、事件循环、QObject 对象树
- Qt 多线程、网络、文件、数据库

验收：
- 能解释 `moveToThread` 为什么比继承 `QThread` 更常推荐
- 复习第 12-29 周面试题

### 第 33 周：简历和 GitHub

练习：
- 简历 1 页、STAR 法则讲项目
- GitHub 置顶 2 个主项目

### 第 34 周：投递和模拟面试

练习：
- 每天投递 5-10 个岗位、2 次模拟面试
- 创建 `job_hunting_strategy.md`

### 第 35 周：复盘、补短板、继续面试

练习：
- 面试后当天写复盘
- 持续投递、迭代

---

## 大丙视频资源索引

以下是大丙视频合集在本计划中的分布：

| 周次 | 大丙视频 | 时长 |
|------|---------|------|
| W1-W4 | C++11 新特性详解（分散补充） | 按需 |
| W5 | C++11 类型推导/右值引用/完美转发 | 按需 |
| W6 | CMake 官方教程（替代失效视频链接） | 按需 |
| W7 | Linux 基础入门教程 | 8.5h |
| W10 | 并发网络通信 — 套接字通信 | 3h |
| W11 | 多线程和线程同步 + IO 多路复用 | 4.5h |
| W12-W14 | Qt 开发入门基础教程 Qt5 | 9.5h |
| W13 | Qt 事件 | — |
| W14 | Qt 按钮使用 + 容器控件 | 4h |
| W15 | Qt 多线程 — 线程池的使用 | 1.5h |
| W16 | Qt 数据库 + Json 在 Qt 中的使用 | — |
| W17 | Qt 多线程网络通信（socket） | 2h |
| W28 | Qt 程序的发布和打包 | — |

**C++11 新特性详解（106集）在 W1-W6 的分布：**

| 部分 | 内容 | 推荐周 |
|------|------|--------|
| 第一部分 | 基础特性（字符串字面量、long long、final/override、static_assert、noexcept） | W1-W2 |
| 第二部分 | 类型推导（auto、decltype、返回类型后置、nullptr、lambda） | W5 |
| 第三部分 | constexpr、using 别名、委托构造、初始化列表、右值引用、move/forward | W5-W6 |
| 第四部分 | 智能指针（shared_ptr、unique_ptr、weak_ptr） | W4 |
| 第五部分 | POD、=default/=delete、友元改进、强类型枚举 | W6 |
| 第六部分 | 并发与多线程（duration、thread、mutex、条件变量、原子操作、future） | W11 |

## 资源链接汇总

| 资源 | 链接 |
|------|------|
| 爱编程的大丙 B站主页 | https://space.bilibili.com/147020887 |
| 大丙 C++11 新特性详解 | https://space.bilibili.com/147020887/lists/6687062 |
| 大丙 Linux 基础入门 | https://www.bilibili.com/video/BV1x5411t7Tj/ |
| CMake 官方教程 | https://cmake.org/cmake/help/latest/guide/tutorial/index.html |
| 大丙 套接字通信 | https://www.bilibili.com/video/BV1We4y197jP/ |
| 大丙 多线程和线程同步 | https://www.bilibili.com/video/BV1Yr4y1U7kR/ |
| 大丙 IO 多路复用 | https://www.bilibili.com/video/BV1XG411X7LX/ |
| 大丙 Qt5 入门基础 | https://www.bilibili.com/video/BV1t44y1q7jC/ |
| simplesoft GDB 调试 | https://space.bilibili.com/391667478/lists/8218249 |
| 大丙博客 | https://subingwen.cn |

## 推荐资料优先级

C++：
- 主线：Claude 生成 example.cpp/exercise.cpp + 大丙 C++11 新特性详解
- 查阅：cppreference.com
- 进阶：侯捷《C++ 面向对象高级编程》

Linux：
- 主线：大丙 Linux 基础入门教程
- 查阅：Linux man pages

网络：
- 主线：湖科大教书匠 TCP/IP + 大丙套接字通信
- 面试：小林 Coding 网络篇

Qt：
- 主线：大丙 Qt5 入门 + Claude 教学代码
- 查阅：Qt 官方文档

调试：
- 主线：simplesoft GDB 调试合集

## 每周最低提交标准

- 至少 3 次 Git commit
- 至少 1 个可运行程序或 1 篇技术笔记
- README 写清楚：环境、构建、运行、截图、已知问题
- 每周复盘 3 件事：学会了什么、卡在哪里、下周怎么改
- 每周五追加 10 个面试题到 `Interview_Questions.md`
