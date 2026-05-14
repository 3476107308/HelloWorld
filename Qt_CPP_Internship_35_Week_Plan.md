# Qt/C++ 实习 35 周学习计划

适用目标：大一基础为 C++ 到面向对象、Linux 基础命令、数据结构，目标为 2027 年 1 月中旬前后争取 Qt/C++ 相关实习。

核心原则：
- 每周至少 5 天学习，每天 2.5-4 小时；周末做验收项目和复盘。
- C++ 不只看语法，要写可编译、可调试、可解释的代码。
- Qt 学习从第 12 周开始，但 CMake、Linux、网络要提前铺垫。
- 每月必须有可展示产物：小程序、README、截图、构建说明。

## 固定周节奏

- 周一到周三：看教程 + 跟写代码。
- 周四：独立重写，不看源码。
- 周五：整理笔记，回答 10 个面试式问题。
- 周六：完成验收任务，提交 Git。
- 周日：复盘、补 bug、写 README 或学习总结。

## 第 1 阶段：C++ 强化，5.13-6.9

### 第 1 周，5.13-5.19：指针、引用、const、内存

学习内容：
- 指针和引用的区别，空指针、野指针、悬垂指针。
- `const int*`、`int* const`、`const int* const`。
- `sizeof`、结构体对齐、栈/堆/全局区/代码区。
- `new/delete`、`new[]/delete[]`，内存泄漏排查。

教程：
- 黑马 C++：指针、引用、内存分区、new/delete。
- 《C++ Primer》第 2、6、12 章选读。
- cppreference：`new expression`、`delete expression`。

练习：
- 手写动态数组类 `IntArray`，支持构造、析构、拷贝、下标访问。
- 画出一个对象在内存中的生命周期。

验收：
- 代码能通过 `g++ -Wall -Wextra` 编译。
- 无越界、无重复释放、无明显泄漏。

### 第 2 周，5.20-5.26：继承、多态、虚函数

学习内容：
- `public/protected/private` 继承和成员访问。
- 虚函数、纯虚函数、抽象类、虚析构函数。
- 静态绑定和动态绑定，虚函数表基本概念。

教程：
- 黑马 C++：继承、多态。
- 侯捷《C++ 面向对象高级编程》：继承与多态部分。
- cppreference：virtual function、override、abstract class。

练习：
- `Animal` 抽象基类，派生 `Cat`、`Dog`、`Bird`。
- 用 `vector<Animal*>` 或 `vector<unique_ptr<Animal>>` 多态调用。

验收：
- 能解释为什么基类析构函数通常要写成 `virtual`。
- 输出每个动物的 `speak()`，并正确释放资源。

### 第 3 周，5.27-6.2：STL 基础

学习内容：
- `vector`、`string`、`map`、`unordered_map`。
- 迭代器、范围 `for`、`sort/find/for_each/count_if`。
- 值语义、拷贝、引用返回的风险。

教程：
- 黑马 C++：STL 初识、常用容器、算法。
- cppreference：`std::vector`、`std::map`、`std::unordered_map`、`std::sort`。

练习：
- 学生信息管理 CLI：增、删、改、查、按成绩排序、按学号查找。

验收：
- 数据结构使用合理，查学号用 `unordered_map`。
- README 写清楚编译和运行方式。

### 第 4 周，6.3-6.9：模板、智能指针、RAII

学习内容：
- 函数模板、类模板、模板实例化。
- `unique_ptr`、`shared_ptr`、`weak_ptr` 的使用场景。
- RAII：资源获取即初始化。

教程：
- 黑马 C++：模板。
- 《C++ Primer》第 16 章选读。
- cppreference：`std::unique_ptr`、`std::shared_ptr`。

练习：
- 写模板类 `Array<T>`。
- 把第 2 周的动物多态改成 `vector<unique_ptr<Animal>>`。

验收：
- 不再手动 `delete` 多态对象。
- 能说清楚 `unique_ptr` 为什么不能复制、只能移动。

## 第 2 阶段：Linux、构建工具、网络，6.10-7.28

### 第 5 周，6.10-6.16：Linux 命令进阶

学习内容：
- `grep/find/tar/chmod/chown/ln/top/ps/netstat/ss`。
- 文件权限、用户/组、软链接和硬链接。
- 输入输出重定向、管道。

教程：
- 尚硅谷 Linux 基础或《Linux 就该这么学》命令部分。
- `man grep`、`man find`、`man chmod`。

练习：
- 写一组命令完成：查日志、打包项目、修改权限、建立软链接。

验收：
- 整理 `linux_commands.md`，每个命令有一个真实例子。

### 第 6 周，6.17-6.23：Linux 进程和信号

学习内容：
- `fork/exec/wait/exit`。
- 进程、父子进程、僵尸进程基本概念。
- `signal/kill/SIGINT/SIGTERM`。

教程：
- Linux 系统编程入门视频：进程、信号章节。
- `man fork`、`man execvp`、`man wait`、`man signal`。

练习：
- C++ 写父进程创建子进程。
- 捕获 Ctrl+C，优雅退出程序。

验收：
- 能画出父子进程执行流程。
- 能解释 `fork()` 返回值的三种情况。

### 第 7 周，6.24-6.30：Shell、Makefile、CMake

学习内容：
- Shell 变量、`if`、`for`、函数、管道。
- Makefile 目标、依赖、变量、伪目标。
- CMake 基础：`add_executable`、`target_include_directories`、`target_link_libraries`。

教程：
- 菜鸟/尚硅谷 Shell 入门作为快速入口。
- GNU Make Manual 前 2 章。
- CMake 官方 Tutorial Step 1-2。

练习：
- Shell 脚本批量重命名文件。
- 用 CMake 管理学生信息管理项目。

验收：
- 项目支持 `cmake -S . -B build` 和 `cmake --build build`。

### 第 8 周，7.1-7.7：网络模型和 IP

学习内容：
- OSI 七层、TCP/IP 四层。
- MAC、IP、子网掩码、网关、路由。
- `ping/traceroute/ipconfig/ifconfig/ip/route`。

教程：
- 湖科大教书匠：计算机网络概述、网络层。
- 《计算机网络：自顶向下方法》网络层选读。

练习：
- 画网络模型图。
- 查询本机 IP、网关、DNS，并解释含义。

验收：
- 能讲清楚“同网段通信”和“跨网段通信”的区别。

### 第 9 周，7.8-7.14：TCP/UDP

学习内容：
- TCP 三次握手、四次挥手。
- 滑动窗口、可靠传输、拥塞控制基本概念。
- UDP 特点和适用场景。

教程：
- 湖科大教书匠：传输层。
- 小林 Coding：TCP/UDP、HTTP 常见面试题。

练习：
- 手画 TCP 连接建立和断开流程。
- 整理 15 个 TCP/UDP 面试问答。

验收：
- 能不用背稿讲清楚握手和挥手。

### 第 10 周，7.15-7.21：Socket 编程

学习内容：
- IP、端口、字节序。
- `socket/bind/listen/accept/connect/send/recv`。
- 阻塞 IO 基本概念。

教程：
- Beej's Guide to Network Programming：前 6 章。
- Linux 网络编程视频：TCP 客户端/服务端。

练习：
- C++ 实现 TCP echo server 和 client。

验收：
- 服务端可接收客户端消息并回显。
- README 包含运行截图和端口说明。

### 第 11 周，7.22-7.28：HTTP 和 DNS

学习内容：
- HTTP 请求行、请求头、请求体。
- GET/POST、常见状态码。
- DNS 解析流程。

教程：
- 小林 Coding：HTTP。
- MDN：HTTP overview、HTTP messages。

练习：
- 用 `telnet` 或 `nc` 手写 HTTP GET。
- 用 C++ socket 请求一个 HTTP 页面。

验收：
- 能解释浏览器输入域名到页面返回的大致流程。

## 第 3 阶段：Qt 入门到核心，7.29-8.25

### 第 12 周，7.29-8.4：Qt 环境、工程结构、基础控件

学习内容：
- 安装 Qt 6 + Qt Creator。
- Qt Widgets 工程结构，CMake 配置。
- `QWidget/QPushButton/QLabel/QLineEdit`。
- `QHBoxLayout/QVBoxLayout/QGridLayout`。

教程：
- Qt 官方：Getting Started Programming with Qt Widgets。
- 黑马或千锋 Qt 入门：Qt Creator、控件、布局。

练习：
- 登录界面：用户名、密码、登录按钮、提示文本。

验收：
- 界面布局不乱，窗口缩放后控件仍合理。
- 项目使用 CMake，而不是只依赖 Qt Creator 自动配置。

### 第 13 周，8.5-8.11：信号槽和元对象系统

学习内容：
- 内置信号槽、自定义信号槽。
- `Q_OBJECT`、moc、元对象系统。
- 事件循环基本概念。
- lambda 连接写法。

教程：
- Qt 官方：Signals & Slots。
- Qt 官方：Meta-Object System。
- 中文视频：信号槽章节重点看 2 遍。

练习：
- 自定义 `LoginWidget`，登录成功发出 `loginSuccess(QString user)` 信号。

验收：
- 能解释 `Q_OBJECT` 少了会出现什么问题。
- 会使用新式 `connect(sender, &Sender::signal, receiver, &Receiver::slot)`。

### 第 14 周，8.12-8.18：Qt 容器、文件、对话框

学习内容：
- `QString/QList/QMap`。
- `QFile/QDir/QTextStream/QDataStream`。
- `QDialog/QMessageBox/QFileDialog`。

教程：
- Qt 官方：QString、QFile、QDir、QDialog 文档。
- 中文 Qt 文件操作、对话框章节。

练习：
- 文本编辑器：新建、打开、保存、另存为、状态提示。

验收：
- 能正确处理文件打开失败、保存失败。
- README 放 2-3 张界面截图。

### 第 15 周，8.19-8.25：Qt 多线程

学习内容：
- `QThread` 基本用法。
- `moveToThread` 推荐模式。
- `QMutex/QWaitCondition/QAtomicInteger`。
- UI 线程不能被阻塞。

教程：
- Qt 官方：QThread、Thread Support in Qt。
- 中文视频：Qt 多线程章节。

练习：
- 后台任务模拟下载，进度条实时更新，UI 不假死。

验收：
- 不在子线程直接操作 UI。
- 线程结束后资源释放正确。

## 第 4 阶段：项目实战，8.26-11.30

### 第 16 周，8.26-9.1：聊天工具项目设计

学习内容：
- `QTcpServer/QTcpSocket`。
- 消息协议设计：登录、群聊、私聊、退出。
- UI 草图和类设计。

教程：
- Qt 官方：QTcpSocket、QTcpServer、Fortune Client/Server 示例。

产出：
- 项目仓库、README、功能列表、类图。

验收：
- 服务端能接受一个客户端连接。

### 第 17 周，9.2-9.8：聊天工具基础通信

学习内容：
- TCP 粘包/半包基本问题。
- 用 JSON 或长度头设计消息。

练习：
- 客户端登录、服务端广播上线消息。

验收：
- 两个客户端能看到彼此上线和群聊消息。

### 第 18 周，9.9-9.15：多人聊天和私聊

学习内容：
- 在线用户列表维护。
- 信号槽解耦网络层和 UI 层。

练习：
- 群聊、私聊、用户列表刷新。

验收：
- 至少 3 个客户端稳定聊天。

### 第 19 周，9.16-9.22：聊天工具收尾

学习内容：
- 日志、异常处理、断线重连基础。
- README 项目包装。

练习：
- 增加聊天记录保存。
- 修复 UI 卡顿和异常断开。

验收：
- GitHub 展示：截图、功能、技术点、构建方式、待优化。

### 第 20 周，9.23-9.29：文件管理器项目设计

学习内容：
- `QFileSystemModel/QTreeView/QTableView`。
- 文件路径、权限、大小、修改时间。

教程：
- Qt 官方：Model/View Programming。
- Qt 官方：QFileSystemModel。

产出：
- 文件浏览界面，可切换目录。

验收：
- 能显示目录树和文件列表。

### 第 21 周，9.30-10.6：文件操作

学习内容：
- 复制、删除、重命名。
- 错误提示和确认对话框。

练习：
- 文件复制/删除/重命名功能。

验收：
- 对不存在路径、权限不足、同名文件有提示。

### 第 22 周，10.7-10.13：调用系统命令和脚本

学习内容：
- `QProcess`。
- 调用 Shell 脚本。
- 权限修改和命令输出显示。

练习：
- 调用 `chmod` 修改权限。
- 执行自定义脚本并显示输出。

验收：
- 不拼接危险命令；参数通过 `QProcess` 参数列表传递。

### 第 23 周，10.14-10.20：文件管理器收尾

学习内容：
- UI 整理、代码分层、README。

练习：
- 增加搜索、收藏目录或最近访问。

验收：
- 项目可演示 5 分钟，有截图、有构建说明。

### 第 24 周，10.21-10.27：Qt HTTP 和 JSON

学习内容：
- `QNetworkAccessManager/QNetworkReply`。
- `QJsonDocument/QJsonObject/QJsonArray`。
- 异步请求和错误处理。

教程：
- Qt 官方：QNetworkAccessManager。
- Qt 官方：JSON Support in Qt。

练习：
- 天气查询工具或 GitHub 用户信息查询工具。

验收：
- 能解析 JSON 并展示关键字段。

### 第 25 周，10.28-11.3：数据库模块

学习内容：
- `QSqlDatabase/QSqlQuery/QSqlTableModel`。
- SQLite 基础。

教程：
- Qt 官方：Qt SQL。

练习：
- 给聊天工具或文件管理器增加 SQLite 历史记录。

验收：
- 数据可增删查改，程序重启后仍保留。

### 第 26 周，11.4-11.10：图形视图和 UI 优化

学习内容：
- `QGraphicsView/QGraphicsScene/QGraphicsItem`。
- 基础绘图和交互。

教程：
- Qt 官方：Graphics View Framework。

练习：
- 做一个简易网络拓扑/文件关系可视化小模块。

验收：
- 能拖动节点，显示节点信息。

### 第 27 周，11.11-11.17：项目重构

学习内容：
- MVC/MVVM 基本思想。
- 业务层、UI 层、网络层分离。
- 命名、头文件依赖、错误处理。

练习：
- 重构两个核心项目的类结构。

验收：
- 每个项目至少整理 3 个明确模块。

### 第 28 周，11.18-11.24：项目打磨

学习内容：
- 日志、配置文件、用户体验细节。
- Release 构建、打包基础。

练习：
- 给两个项目增加配置保存、日志输出。

验收：
- 项目能在新目录重新构建运行。

### 第 29 周，11.25-11.30：作品集整理

学习内容：
- GitHub README 写法。
- 项目讲解结构：背景、功能、技术、难点、优化。

练习：
- 录制 1-2 分钟演示视频或准备 GIF。

验收：
- 两个主项目和一个小工具都能放简历。

## 第 5 阶段：面试和投递，12.1-1 月中旬

### 第 30 周，12.1-12.7：C++ 面试专题

学习内容：
- 指针/引用/const/static。
- 虚函数、多态、虚析构。
- STL、迭代器失效、智能指针。

练习：
- 牛客 C++ 基础题。
- 手写 30 个 C++ 高频问答。

验收：
- 能完整讲出一个对象从构造到析构的过程。

### 第 31 周，12.8-12.14：Linux 和网络面试专题

学习内容：
- Linux 命令、进程、信号、权限。
- TCP/UDP/HTTP/DNS。

练习：
- 每天 10 道网络/Linux 问答。
- 复盘 socket 项目。

验收：
- 能手画 TCP、HTTP、DNS 流程图。

### 第 32 周，12.15-12.21：Qt 面试专题

学习内容：
- 信号槽、元对象系统、事件循环。
- QObject 对象树、父子对象内存管理。
- Qt 多线程、网络、文件、数据库。

练习：
- 整理 Qt 高频 50 问。
- 讲解项目里的信号槽链路。

验收：
- 能解释 `moveToThread` 为什么比继承 `QThread` 写业务逻辑更常推荐。

### 第 33 周，12.22-12.28：简历和 GitHub

学习内容：
- 简历项目描述。
- STAR 法则讲项目。

练习：
- 简历 1 页。
- GitHub 置顶 2 个主项目。

验收：
- 每个项目描述包含：技术栈、功能、难点、你的贡献、结果。

### 第 34 周，12.29-1.4：投递和模拟面试

学习内容：
- Qt/C++ 实习岗位 JD 分析。
- 自我介绍、项目介绍、反问准备。

练习：
- 每天投递 5-10 个岗位。
- 做 2 次模拟面试。

验收：
- 准备 3 分钟项目讲解和 30 秒自我介绍。

### 第 35 周，1.5-1 月中旬：复盘、补短板、继续面试

学习内容：
- 根据面试反馈补弱项。
- 项目 bug 修复和简历微调。

练习：
- 面试后当天写复盘。
- 把不会的问题整理成专题笔记。

验收：
- 持续投递、复盘、迭代，争取寒假前后拿到面试或 offer。

## 推荐资料优先级

C++：
- 主线：黑马 C++ + 《C++ Primer》第 5 版。
- 查阅：cppreference。
- 进阶：侯捷《C++ 面向对象高级编程》。

Linux：
- 主线：尚硅谷 Linux 基础或《Linux 就该这么学》。
- 查阅：Linux man pages。
- 重点：命令、权限、进程、信号、Shell。

网络：
- 主线：湖科大教书匠 TCP/IP。
- 实战：Beej's Guide to Network Programming。
- 面试：小林 Coding 网络篇。

Qt：
- 主线：Qt 官方文档 + 黑马/千锋 Qt 实战教程。
- 必看官方主题：Qt Widgets、Signals & Slots、Meta-Object System、QThread、QTcpSocket/QTcpServer、QNetworkAccessManager、Model/View、Qt SQL。

## 简历项目建议

项目 1：Qt 网络聊天工具
- 技术栈：Qt Widgets、QTcpServer、QTcpSocket、信号槽、多线程、JSON、SQLite。
- 亮点：多人聊天、私聊、消息广播、断线处理、聊天记录。

项目 2：Qt 文件管理工具
- 技术栈：Qt Widgets、QFile/QDir、QFileSystemModel、QProcess、Linux 命令、Shell。
- 亮点：文件浏览、复制删除重命名、权限修改、搜索、最近访问。

项目 3：Qt HTTP 小工具
- 技术栈：QNetworkAccessManager、QJsonDocument、异步请求。
- 亮点：API 调用、错误处理、JSON 展示。

## 每周最低提交标准

- 至少 3 次 Git commit。
- 至少 1 个可运行程序或 1 篇技术笔记。
- README 写清楚：环境、构建、运行、截图、已知问题。
- 每周复盘 3 件事：学会了什么、卡在哪里、下周怎么改。
