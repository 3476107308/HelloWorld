# C++ Qt 开发相关学习路线

> 整理人：计算机小赛同学（窦森奇）
> 路线：Qt 桌面/跨平台应用开发方向，从基础到项目实战的完整学习路径。

---

## 阶段一：主线基础篇

### 1. C 语言基础

| 知识模块 | 核心内容 |
|---|---|
| 数据类型与运算符 | 基本数据类型、运算符表达式、类型转换 |
| 控制结构 | 顺序、分支、循环结构 |
| 函数 | 函数定义与声明、参数传递、递归 |
| 数组与字符串 | 一维/多维数组、字符数组与字符串处理 |
| 指针 | 指针基础、指针与数组、函数指针、指针数组 |
| 结构体与联合 | struct 定义与使用、union、enum |
| 文件操作 | 文件读写、二进制文件处理 |
| 内存管理 | malloc/free、内存布局（堆/栈/数据段/代码区） |
| 数据结构实现 | 用 C 语言实现链表、栈、队列、树等基础数据结构 |

**学习目标：** 掌握 C 语言核心语法，深入理解指针与内存模型，为 Qt 底层源码阅读打好基础。
**与 Qt 的关联：** Qt 底层大量使用 C 语言特性，理解指针与内存管理对排查 Qt 程序内存泄漏至关重要。

### 2. 数据结构初阶

| 知识模块 | 核心内容 |
|---|---|
| 线性表 | 顺序表、链表的实现与操作 |
| 栈与队列 | 顺序栈/链栈、循环队列、双端队列 |
| 树 | 二叉树、二叉搜索树、堆 |
| 图 | 图的存储与遍历（BFS/DFS） |
| 排序算法 | 冒泡、选择、插入、快排、归并、堆排序 |
| 查找算法 | 顺序查找、二分查找、哈希表 |
| 复杂度分析 | 大 O 表示法、时间/空间复杂度分析 |

**学习目标：** 理解常用数据结构原理，掌握经典算法，为 Qt 自定义数据模型与高效 UI 渲染打好基础。
**与 Qt 的关联：** Qt 的 QList、QVector、QMap、QHash 等容器与 STL 数据结构一脉相承，理解底层数据结构有助于正确选择 Qt 容器。

### 3. C++ 基础

| 知识模块 | 核心内容 |
|---|---|
| C++ 基础 | 命名空间、引用、函数重载、默认参数 |
| 类与对象 | 封装、构造/析构函数、拷贝构造、赋值运算符重载 |
| 动态内存 | new/delete、深层拷贝、移动构造（右值引用） |
| 继承与多态 | 单继承/多继承、虚函数、虚析构、构造顺序与虚函数表 |
| 运算符重载 | 算术/关系/下标/函数调用运算符重载 |
| 模板 | 函数模板、类模板、模板特化 |
| STL 基础 | vector、list、map、set、algorithm 常用算法 |
| 异常处理 | try/catch/throw、异常安全 |
| 类型转换 | static_cast、dynamic_cast、const_cast、reinterpret_cast |

**学习目标：** 掌握 C++ 面向对象编程思想，深入理解封装/继承/多态，能使用 STL 进行高效开发。
**与 Qt 的关联：** Qt 框架本身基于 C++ 面向对象设计，信号与槽机制依赖虚函数与模板技术，QObject 的对象树管理依赖继承体系，掌握 C++ 是学习 Qt 的前提。

### 4. C++ 进阶（C++11 新特性）

| 知识模块 | 核心内容 |
|---|---|
| C++11 新特性 | auto、decltype、范围 for、lambda 表达式 |
| 智能指针 | unique_ptr、shared_ptr、weak_ptr、引用计数 |
| 右值引用与移动语义 | std::move、std::forward、完美转发 |
| 并发编程 | std::thread、mutex、atomic、condition_variable |
| 类型推导与模板增强 | constexpr、类型别名、变参模板 |

**学习目标：** 深入理解现代 C++ 核心特性，掌握智能指针与移动语义，编写更高效安全的代码。
**与 Qt 的关联：** Qt 5.7+ 全面支持 C++11。了解智能指针有助于结合 QSharedPointer/QScopedPointer 做内存管理；lambda 表达式在 Qt 信号槽连接和线程任务中广泛使用。
**侧重点：** 一定要把 C++11 学明白，市场主流是 Qt5，弄明白 Qt5 优先。

### 5. 23 种常用设计模式学习

- **创建型模式：**
  - a. 单例模式
  - b. 简单工厂模式
  - c. 工厂模式
  - d. 抽象工厂模式
  - e. 生成器/建造者模式
  - f. 原型模式
- **结构型模式：**
  - a. 适配器模式
  - b. 桥接模式
  - c. 组合模式
  - d. 装饰模式
  - e. 外观模式
  - f. 享元模式
  - g. 代理模式
- **行为型模式：**
  - a. 责任链模式
  - b. 命令模式
  - c. 迭代器模式
  - d. 中介者模式
  - e. 备忘录模式
  - f. 观察者模式
  - g. 策略模式
  - h. 状态模式
  - i. 模板方法模式
  - j. 访问者模式

**侧重点——与 Qt 的关联、应优先学会：** 观察者模式（最重要）、单例模式、工厂模式、装饰模式、代理模式、命令模式、组合模式、适配器模式。

---

## 阶段二：Qt 核心技术篇

### 6. Qt 基础入门

| 知识模块 | 核心内容 |
|---|---|
| Qt 环境搭建 | Qt Creator 安装、Qt 版本选择（Qt5/Qt6）、编译器配置（MinGW/MSVC） |
| 第一个 Qt 程序 | 项目结构、main 函数、QApplication 事件循环 |
| 窗口与控件 | QWidget、QMainWindow、QDialog、窗口属性设置 |
| 布局管理器 | QHBoxLayout、QVBoxLayout、QGridLayout、QFormLayout、弹簧 spacer |
| 常用控件 | QPushButton、QLabel、QLineEdit、QTextEdit、QComboBox、QListView、QTableView、QTreeWidget |
| 字符串处理 | QString 常用操作、QString 与 std::string 转换、中文编码处理 |
| Qt 容器类 | QList、QVector、QMap、QHash、QSet 与 STL 容器的对比 |

**学习目标：** 搭建 Qt 开发环境，掌握窗口与常用控件的使用，能使用布局管理器完成基本 UI 界面设计。

### 7. 信号与槽机制

| 知识模块 | 核心内容 |
|---|---|
| 信号与槽基础 | signals/slots 关键字、connect 函数、信号发出 emit |
| 自定义信号与槽 | 自定义信号声明、自定义槽函数、参数传递 |
| 连接方式 | Qt4 字符串连接 vs Qt5 函数指针连接 vs Lambda 连接 |
| 信号重载处理 | QOverload::of 选择重载信号、函数指针类型匹配 |
| 第三方信号槽 | QSignalMapper（Qt5）、信号映射、一对多/多对一连接 |
| 断开连接 | disconnect 使用场景、自动断开机制（Lambda 版本） |
| 信号与槽原理 | MOC 元对象编译器、moc 文件分析、信号槽底层实现 |

**学习目标：** 深入理解 Qt 信号与槽机制，能灵活使用各种连接方式，理解 MOC 原理。

### 8. 事件系统

| 知识模块 | 核心内容 |
|---|---|
| 事件机制基础 | QEvent 类、事件循环、QEventLoop、事件分发 |
| 常见事件类型 | 鼠标事件（QMouseEvent）、键盘事件（QKeyEvent）、定时器事件（QTimerEvent）、绘制事件（QPaintEvent） |
| 事件处理函数 | event()、eventFilter() 事件过滤、自定义事件处理 |
| 自定义事件 | QEvent 子类、postEvent/sendEvent、自定义事件类型注册 |
| 事件过滤器 | installEventFilter、全局事件过滤、对象间事件拦截 |
| 拖放机制 | dragEnterEvent/dropEvent、QMimeData、拖放操作类型 |

**学习目标：** 掌握 Qt 事件系统工作原理，能处理各类用户交互事件，能编写自定义事件与事件过滤器。

### 9. 绘图与自定义控件

| 知识模块 | 核心内容 |
|---|---|
| QPainter 基础 | 绘制基本图形（线/矩形/椭圆/多边形）、画笔 QPen、画刷 QBrush |
| 渐变与变换 | 线性渐变/径向渐变/锥形渐变、坐标变换（平移/旋转/缩放） |
| 文字与路径 | QFont 字体设置、QPainterPath 路径绘制、文字排版 |
| 图片绘制 | QImage/QPixmap/QBitmap/QPicture 区别与使用、图片缩放与变换 |
| 自定义控件 | QWidget 自定义绘制、控件属性设计（Q_PROPERTY）、样式表 QSS |
| 图形视图框架 | QGraphicsScene/QGraphicsView/QGraphicsItem、场景坐标系、碰撞检测 |
| OpenGL 集成 | QOpenGLWidget、基本 OpenGL 渲染、着色器编程入门 |

**学习目标：** 掌握 Qt 2D 绘图技术，能开发自定义控件与图表组件，了解图形视图框架与 OpenGL 集成。

### 10. 文件与数据存储

| 知识模块 | 核心内容 |
|---|---|
| 文件 I/O | QFile 读写、QTextStream 文本流、QDataStream 二进制流 |
| 文件信息 | QFileInfo 文件属性、QDir 目录操作、QFileSystemWatcher 文件监控 |
| 临时文件 | QTemporaryFile/QTemporaryDir 使用场景 |
| 序列化 | 对象序列化与反序列化、QDataStream 二进制序列化、JSON 序列化（QJsonDocument） |
| 配置文件 | QSettings 读写 INI/注册表、XML 配置文件解析（QXmlStreamReader） |
| 数据库操作 | Qt SQL 模块、QSqlDatabase 连接管理、QSqlQuery 执行 SQL、QSqlTableModel/QSqlRelationalTableModel、Qt 中操作 MySQL/SQLite 数据库 |
| 数据库连接池 | 自实现 Qt 数据库连接池、MySQL/SQLite 连接管理 |

**学习目标：** 掌握 Qt 文件读写与数据持久化方案，能使用 Qt SQL 模块操作数据库，理解序列化机制。

### 11. 网络编程

| 知识模块 | 核心内容 |
|---|---|
| 网络基础 | OSI 模型、TCP/UDP 协议基础 |
| Qt 网络模块 | QNetworkAccessManager HTTP 请求、QNetworkReply 响应处理 |
| TCP 通信 | QTcpServer/QTcpSocket、服务器与客户端实现 |
| UDP 通信 | QUdpSocket、广播与组播 |
| HTTP/HTTPS | GET/POST 请求、文件上传下载、SSL 配置 |
| WebSocket | QWebSocket、实时双向通信 |
| 与 Linux 网络编程的关系 | Qt 封装 vs 原生 Socket API、epoll 在 Qt 底层的使用 |

**学习目标：** 掌握 Qt 网络编程接口，能实现 TCP/UDP/HTTP 通信，理解 Qt 网络模块与底层 Socket 的关系。

### 12. 多线程编程

| 知识模块 | 核心内容 |
|---|---|
| QThread 基础 | QThread 创建与使用、moveToThread 模式、run() 重写 |
| 线程安全 | QMutex/QMutexLocker、QReadWriteLock、QSemaphore、QWaitCondition |
| 任务队列 | QRunnable + QThreadPool 线程池模式 |
| QtConcurrent | QtConcurrent::run/map/filtered、QFuture/QFutureWatcher 异步编程 |
| 跨线程通信 | 信号与槽跨线程连接（QueuedConnection）、QMetaObject::invokeMethod |
| 线程与事件循环 | 线程内事件循环、子线程中使用 QTimer/QNetworkAccessManager |
| 常见陷阱 | 线程中操作 UI 的禁忌、对象线程归属（threadAffinity） |

**学习目标：** 掌握 Qt 多线程编程的各种方式，能编写线程安全的 Qt 应用，避免常见多线程陷阱。

---

## 阶段三：Qt 高级技术篇

### 13. QML 与 Qt Quick

| 知识模块 | 核心内容 |
|---|---|
| QML 语法基础 | QML 元素声明、属性绑定、信号处理、组件复用 |
| Qt Quick 控件 | QtQuick.Controls、Button/TextField/Slider/ScrollView 等 |
| 布局与定位 | Row/Column/Grid/Repeater、锚定布局 anchors |
| 动画与过渡 | PropertyAnimation/NumberAnimation、状态切换、过渡动画 |
| C++ 与 QML 交互 | Q_PROPERTY/Q_INVOKABLE、QML 注册 C++ 类型、上下文属性 |
| 模型/视图 | QML ListView/GridView、C++ QAbstractListModel 暴露数据 |
| Qt Quick 3D | 基础 3D 场景搭建、模型加载、材质与光照 |

**学习目标：** 掌握 QML 声明式 UI 编程，能实现流畅的动画效果与现代化界面设计，理解 C++ 与 QML 交互机制。

### 14. 模型/视图架构

| 知识模块 | 核心内容 |
|---|---|
| 模型/视图概述 | Model/View 架构设计思想、数据与表现分离 |
| 预定义模型 | QStringListModel、QStandardItemModel |
| 自定义模型 | QAbstractItemModel/QAbstractListModel/QAbstractTableModel 子类化 |
| 视图类 | QListView/QTableView/QTreeView、自定义委托 QStyledItemDelegate |
| 代理/委托 | 自定义绘制委托、编辑器委托、QSortFilterProxyModel 排序过滤 |
| 选择模型 | QItemSelectionModel、多选/单选/扩展选择模式 |
| 数据更新 | dataChanged 信号、beginInsertRows/endInsertRows 模型更新 |

**学习目标：** 深入理解 Qt Model/View 架构，能开发自定义数据模型与视图委托，处理大规模数据展示。

### 15. Qt 高级特性

| 知识模块 | 核心内容 |
|---|---|
| 元对象系统 | Q_OBJECT 宏、MOC 元对象编译器、属性系统 Q_PROPERTY |
| 动态属性 | setProperty/property、运行时动态属性 |
| 反射机制 | QMetaObject::invokeMethod、className、方法枚举 |
| 对象树管理 | QObject 父子对象树、自动销毁机制、内存泄漏排查 |
| 插件机制 | Qt 插件框架、QPluginLoader、自定义插件接口设计 |
| 国际化 | tr() 函数、Qt Linguist 工具、多语言切换 |
| 样式表 | QSS 语法、伪状态、子控件样式、主题切换 |
| 单元测试 | Qt Test 模块、QTest::qExec、数据驱动测试、QSignalSpy 信号验证 |

**学习目标：** 掌握 Qt 元对象系统与高级特性，能开发支持插件化和国际化的专业级应用。

---

## 阶段四：工具与协作篇

### 16. Git 课程学习

| 知识模块 | 核心内容 |
|---|---|
| 版本控制基础 | Git 工作区/暂存区/版本库、基本操作 |
| 分支策略 | Git Flow、feature 分支管理 |
| 协作工作流 | Pull Request、Code Review、冲突解决 |
| CI/CD 集成 | Git Hooks、自动化构建与测试 |

**学习目标：** 熟练使用 Git 进行团队协作开发，掌握分支管理策略。

### 17. Docker 学习或者 Inno Setup Compiler 学习（二选一）

| 知识模块 | 核心内容 |
|---|---|
| 容器技术基础 | Docker 架构、镜像与容器 |
| 镜像构建 | Dockerfile 编写、Qt 开发环境容器化 |
| 部署 | Qt 应用打包与容器化部署 |
| 打包全套 | Inno Setup Compiler 可以完成 Qt 应用程序的完全打包 |

**学习目标：** 了解容器化技术，能用 Docker 部署 Qt 应用及开发环境。

---

## 阶段五：项目实战篇

### 18. Qt 项目实战方向

| 实战项目 | 核心技术 | 项目描述 |
|---|---|---|
| 即时通讯系统 | TCP/UDP、多线程、数据库、自定义控件 | 仿 QQ/微信客户端，实现注册登录、好友列表、一对一/群组聊天、文件传输、消息离线存储 |
| 音视频播放器 | FFmpeg 集成、QOpenGLWidget、多线程、事件系统 | 基于 FFmpeg 的多媒体播放器，支持多种格式、播放控制、进度条、音量调节、播放列表管理 |
| 数据可视化平台 | Model/View、QChart、QML、网络请求 | 实时数据采集与图表展示系统，支持折线/柱状/饼图，数据来源可配置（串口/网络/数据库） |
| 串口调试工具 | Qt 串口模块（QSerialPort）、自定义控件、文件 I/O | 仿 XCOM 串口调试助手，支持多串口、波特率配置、定时发送、数据收发日志、CRC 校验 |
| 代码编辑器 | QTextEdit/QPlainTextEdit、语法高亮、文件树、插件机制 | 仿 Notepad++ 轻量级编辑器，支持多标签页、语法高亮、查找替换、文件浏览树 |
| 屏幕录制/截图工具 | QPainter、QScreen、事件系统、系统托盘 | 仿 Snipaste 截图工具，支持区域截图、标注绘制、贴图、快捷键、系统托盘 |
| 物联网监控面板 | MQTT/WebSocket、QML、Model/View、数据库 | IoT 设备数据监控看板，实时展示传感器数据、告警通知、历史数据查询、设备管理 |

**学习目标：** 通过完整项目实战，综合运用 Qt 各模块知识，具备独立开发中等复杂度桌面应用的能力。

---

## C++ Qt 开发学习路线总览图

**第一阶段：主线基础篇（3-4 个月）**
- C 语言基础
- 数据结构初阶
- C++ 基础
- C++ 进阶（C++11/14/17）

**第二阶段：Qt 核心技术篇（4-5 个月）**
- Qt 基础入门（环境搭建、窗口控件、布局）
- 信号与槽机制
- 事件系统
- 绘图与自定义控件
- 文件与数据存储（含 Qt SQL）
- 网络编程（TCP/UDP/HTTP）
- 多线程编程

**第三阶段：Qt 高级技术篇（2-3 个月）**
- QML 与 Qt Quick
- 模型/视图架构
- Qt 高级特性（元对象系统、插件、国际化、测试）

**第四阶段：工具与协作篇（1-2 个月）**
- Git 学习
- Docker 学习或者 Inno Setup Compiler 学习（二选一）

**第五阶段：项目实战篇（2-3 个月）**
- Qt 项目实战（选 1-2 个具体方向深入）
  - 即时通讯系统
  - 音视频播放器
  - 数据可视化平台
  - 串口调试工具
  - 代码编辑器
  - 屏幕录制/截图工具
  - 物联网监控面板

---

## 推荐学习顺序

C 语言 → 数据结构 → C++ 基础 → C++ 进阶（C++11/14/17）→ 23 种常用的设计模式
→ Qt 基础入门 → 信号与槽 → 事件系统 → 绘图与自定义控件
→ 文件与数据存储 → 网络编程 → 多线程编程
→ QML & Qt Quick → 模型/视图架构 → Qt 高级特性
→ Git → Docker 学习或者 Inno Setup Compiler 学习（二选一）
→ Qt 项目实战

---

## 就业方向与岗位

| 岗位方向 | 典型公司/行业 | 核心技能要求 |
|---|---|---|
| 桌面应用开发 | WPS、福昕、墨刀 | Qt Widgets、自定义控件、跨平台打包 |
| 医疗/工业软件 | 迈瑞医疗、联影医疗、大疆 | Qt + OpenGL、串口通信、实时数据处理 |
| 车载系统开发 | 蔚来、小鹏、理想、博世 | QML/Qt Quick、Qt Quick 3D、车载 HMI |
| 音视频客户端 | Zoom、腾讯会议、声网 | FFmpeg + Qt、OpenGL 渲染、实时通信 |
| 嵌入式 GUI 开发 | 海康威视、大华、华为 | Qt for Embedded Linux、帧缓冲、触摸交互 |
| 金融交易终端 | 同花顺、东方财富、华泰证券 | Qt + 网络编程、数据可视化、高频刷新 |
| 游戏客户端工具 | 游戏公司（内部工具链） | Qt + 编辑器开发、插件架构、脚本集成 |

---

**整理人：** 计算机小赛同学（窦森奇）
