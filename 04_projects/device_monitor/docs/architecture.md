# device_monitor 架构记录

## 当前状态

- 阶段 0 工程骨架已创建，源码可以用 Qt 6.11.0 + MinGW 13.1.0 + CMake/Ninja 配置并编译。
- 参考样本：`D:\AAA study\sandbox\reference_projects\SerialTest`。
- 技术选择：Qt 6 + CMake；正式项目不使用 qmake。

> **构建方式（2026-09-19 更新 —— 本文档下方历史记录里的命令已作废）**
> 仓库已从 `D:\AAA study`（带空格）改名为 `D:\AAA_study`（下划线）。CMake 会**把源目录绝对路径写死进 `CMakeCache.txt` 和 `build.ninja`**，所以改名之前存在的构建目录（`build-windows/`、`build-windows-qt/`、`build/Desktop_Qt_6_11_0_MinGW_64_bit-Debug/`）**全部失效**，直接构建会报 `CMakeCache.txt directory ... is different`。**CMake 构建目录不可搬迁**——源目录一改名，就必须删掉重新生成。
> 当前统一使用以下任一种，**不要再用历史记录里的 `cmake --build build-windows-qt -j2`**：
> - **命令行**：`powershell -ExecutionPolicy Bypass -File tools\build.ps1`（构建目录 `build-qt/`）
> - **Qt Creator**：kit `Desktop_Qt_6_11_0_MinGW_64_bit-Debug`
>
> 脚本会把 `C:\Qt\Tools\mingw1310_64` 放到 PATH 最前。原因：系统 PATH 上另外有一个 MSYS2 的 `g++`（GCC 15.2.0），而 Qt 6.11.0 的 `mingw_64` 是用 **GCC 13.1.0** 构建的，用错编译器会产生难以理解的链接错误。

阶段 0 文件：

- `CMakeLists.txt`
- `include/mainwindow.h`
- `src/main.cpp`
- `src/mainwindow.cpp`

阶段 1 当前文件：

- `include/device.h`
- `include/mainwindow.h`
- `src/mainwindow.cpp`

阶段 0 验收记录：

- 配置：`cmake -S . -B build-windows-qt -G Ninja -DCMAKE_PREFIX_PATH=C:\Qt\6.11.0\mingw_64`
- 编译：`cmake --build build-windows-qt -j2`
- 结果：`device_monitor.exe` 编译成功，使用 `QT_QPA_PLATFORM=offscreen` 可进入事件循环。
- 注意：系统 `C:\msys64\ucrt64` 工具链当前无法通过 CMake 的最小编译器自检；Qt 自带 `C:\Qt\Tools\mingw1310_64` 工具链正常。

## 目标依赖关系

```text
main
  -> MainWindow
      -> DeviceStore       (设备数据和状态)
      -> DeviceModel       (阶段 3 再引入)
      -> Simulator         (QTimer 模拟数据)
      -> Transport         (阶段 5，引入串口/TCP)
      -> LogService        (日志和历史记录)
```

## 设计约束

- UI 不直接负责串口协议解析。
- 模拟数据和真实通信数据必须最终进入同一套设备数据更新接口。
- 能用 Qt 对象父子关系管理的对象，不自行裸 `new/delete`。
- 线程只在有可观察的阻塞或性能问题后引入。
- 每增加一个模块，都要有一个可运行验收场景。

## 参考样本映射

| SerialTest 模块 | 本项目暂定对应 | 学习重点 |
| --- | --- | --- |
| `mainwindow.*` | `MainWindow` | 窗口组织和 UI 事件连接 |
| `connection.*` | `Transport` | 通信抽象、错误和数据到达信号 |
| `devicetab.*` | `DevicePanel` | 设备选择和参数展示 |
| `datatab.*` | `LogView` | 原始数据/日志展示 |
| `plottab.*` | `TelemetryPlot` | 历史数据和刷新边界 |
| `mysettings.*` | `SettingsStore` | 配置持久化 |

这些是职责映射，不是代码复制清单。实现时优先保持小规模和可解释性。

## 模块职责

> **"不负责"一列比"负责"更重要** —— 它记录的是这个项目里划过的**边界**，也是面试时"为什么这么设计"的答案。

| 模块 | 负责 | 不负责 |
|---|---|---|
| `ConnectBar` | 让用户填 host/port；**校验合法性**（空 / 非数字 / 超出 1–65535）；显示连接状态（灰/橙/绿/红）；**打开中禁止重复点击** | ❌ **不知道 `Transport` 存在**（通过 `openRequested` / `closeRequested` 信号把请求发出去）<br>❌ 不写日志<br>❌ 不发起真正的连接 |
| `DataView` | 显示收到的字节；**文本 / HEX 两种模式**；`rx` / `tx` 字节计数；发送区（输入框 + **追加 `\r\n`** 选项） | ❌ **不知道 TCP / 串口存在**（只认 `QByteArray`）<br>❌ 不解析 JSON<br>❌ 不写日志<br>❌ 不判断连接状态 |
| **`Transport`**（抽象） | 定义"传输"该有的能力：`open` / `close` / `isConnected` / `sendByte`；五个对外信号 `receiveByte` / `openSucceeded` / `openError` / `closed` / `sendFailed` | ❌ 不知道界面<br>❌ 不知道数据格式（JSON）<br>❌ 不知道背后是 TCP 还是串口 —— **这正是抽象层的意义** |
| `TcpTransport` | 用 `QTcpSocket` 实现 `Transport`；`connectToHost` 异步连接；`state()` 状态判断；`readyRead` → `readAll()` | ❌ 不知道界面<br>❌ 不知道数据格式<br>❌ 不做分包（那是 `JsonLineParser` / `Framer` 的活） |
| `JsonLineParser` | **字节流 → 完整消息**（内部缓冲区 + 循环切 `\n`，解决粘包/半包）→ `QJsonObject`；解析失败发 `parseFailed` 而不崩 | ❌ 不知道界面<br>❌ 不知道传输方式<br>❌ **不做任何 I/O**（只处理传进来的字节） |
| `MainWindow` | 组装界面（连接条 / 收发框 / 曲线 / 日志 / 设备面板）；**充当「中枢」—— 把 A 的信号翻译成对 B 的方法调用**（例：`Transport::openError` → 写日志 + `connect_bar_->setStatus(...)`） | ❌ 不做协议解析<br>❌ 不做网络 I/O<br>❌ 不实现传输细节 |

### 两个贯穿全项目的原则

1. **数据单向流动，边界只认抽象**：`QTcpSocket` → `TcpTransport` → **`Transport`** → `MainWindow` → 界面。
   上层永远只依赖 `Transport`，所以 Day 11 加串口时 `MainWindow` 一行都不用改。

2. **信号向上、方法调用向下，都不越级**：
   - 下层需要通知上层 → **信号**（`ConnectBar::openRequested`、`Transport::receiveByte`）
   - 上层需要更新下层显示 → **方法调用**（`connect_bar_->setStatus()`、`data_view_->appendData()`）
   - **同层组件之间（`ConnectBar` ↔ `DataView`）永不直接通信**，一律经过 `MainWindow` 中转

## 线程模型

> **一句话**：把"收字节 + 分包 + 解析"搬到独立线程，UI 线程只负责"显示"。
> 目标是硬性验收第 3 条：**接收线程与 UI 线程分离，实测 ≥ 10 万字节/秒时 UI 仍可交互。**

### 1. 为什么要开线程

**不开线程时，界面为什么会卡？**

主线程在跑一个**事件循环**：不断"取出一个事件 → 处理它 → 再取下一个"。鼠标点击、键盘、定时器、socket 有数据，全都是从这个循环里被取出来处理的。

**如果某个处理函数执行太久，循环就卡在"处理它"这一步出不来** —— 期间到达的点击、重绘请求全都排在队列里，没人去取。

**注意：程序并没有死，它还在跑那个函数 —— 只是腾不出手响应界面。**

**具体现象：**

> 主线程正在处理一个函数，用户点击了按钮，但主线程对此没有反应 —— 因为它在处理函数中。
> 但用户不知道，于是**多次点击**，最终 Windows 弹出"程序无响应"的提示。
> 用户以为程序挂了就关掉它 —— **其实程序还在正常运行，只是没空响应。**

### 2. 两个线程各干什么

```text
IO 线程（IoWorker）                   UI 线程（MainWindow）
──────────────────────────────       ──────────────────────────────
接收字节（readyRead → readAll）        处理用户操作（点"打开"、输入、勾选）
分包（按 \n 切出完整消息）              刷新界面（收发框、曲线、日志、状态灯）
解析（JSON → QJsonObject）             维护连接状态显示
```

**分界线一句话：**

> **IO 线程绝不碰任何 `QWidget`；UI 线程绝不碰 socket。**

**为什么必须这样（本节最重要的一条）：**

> Qt 对"UI 的线程归属"**只有约定（contract），没有强制（enforcement）**：
>
> - **编译期查不了** —— C++ 的类型系统不携带"对象属于哪个线程"的信息，`data_view_->appendData(bytes)` 在语法和类型上完全合法
> - **运行期不查** —— 给每次 UI 调用加线程检查的开销太大，Qt 不做
>
> 所以违反了它**不会当场报错**，但结果属于**未定义行为** —— 可能当场崩溃、可能花屏、**也可能看起来完全正常，直到某天数据量变大或时序变了才突然崩**。
>
> **这正是它比"崩溃"更危险的地方：崩溃会当刻告诉你，未定义行为会一直骗你。**

### 3. 两个线程之间怎么传数据

**机制：`Qt::QueuedConnection`（队列连接）**

跨线程的 `connect` **自动**使用队列连接，不需要手写这个参数。

**参数发生了什么：被拷贝**

`emit dataReceived(bytes)` 时，Qt 把参数**拷贝一份**，投递到接收线程的**事件队列**；等那边的事件循环取出来，才调用槽函数。

**好处与代价：**

| | 说明 |
|---|---|
| **好处 —— 无数据竞争** | 两个线程操作的是**不同的副本**，不存在"同时改同一块内存" |
| **代价 —— 拷贝开销** | 内存占用翻倍，大流量下拷贝本身要花时间（这正是后续「批量刷新」要优化的点） |

### 4. 线程的生命周期

**启动（四步）：**

```cpp
auto* io_thread = new QThread(this);      // ① 建线程对象（注意：它住在【主线程】）
auto* worker = new IoWorker;              // ② 建 worker —— 【不能传 parent】
worker->moveToThread(io_thread);          // ③ 把 worker 的"线程归属"搬到子线程
connect(io_thread, &QThread::started,  worker, &IoWorker::start);
connect(io_thread, &QThread::finished, worker, &QObject::deleteLater);
io_thread->start();                       // ④ 启动
```

**两个必须理解的点：**

1. **`worker` 不能有 parent** —— 因为**父对象决定子对象的线程归属**。有 parent 的对象，线程归属已经被 parent 定死，`moveToThread` 搬不动它。
2. **`QThread` 对象 ≠ 线程** —— 它只是"管理器"（相当于工牌），住在**创建它的那个线程**（主线程）；真正的操作系统线程由 `start()` 时创建。

**结束：**

```cpp
io_thread->quit();        // 请求子线程的事件循环退出（异步）
io_thread->wait();        // 阻塞等待它真正结束
```

**为什么必须 `wait()`：**

> `quit()` 只是**请求**。线程还要**跑完当前正在执行的槽函数** → 退出事件循环 → `run()` 返回，**这时才真正结束**。
> 如果不等它就继续往下走（比如销毁 `IoWorker`、或让 `MainWindow` 析构），**线程可能还在访问已经析构的对象** → 崩溃，而且崩得不规律。
>
> **根因：`quit()` 是异步的，`delete` 是同步的。** 线程什么时候结束，由线程自己决定，不由你决定。
>
> `worker` 的销毁交给 `connect(io_thread, &QThread::finished, worker, &QObject::deleteLater)` —— **让它在自己的线程里销毁自己**，这是唯一安全的方式（你不能从主线程 `delete` 一个属于子线程的对象）。

## 当前进度记录

日期：2026-09-05  
功能：完成模拟设备数据的基础接入和设备选择详情更新。  
新增/修改文件：`include/device.h`、`include/mainwindow.h`、`src/mainwindow.cpp`、`CMakeLists.txt`。  
构建命令：`cmake --build build-windows-qt -j2`。  
运行证据：构建和链接成功；窗口显示 3 个设备，点击列表行后右侧显示对应名称、在线状态、温度和电压。  
遇到的问题：结构体聚合初始化字段类型、`QStringLiteral`、`QLabel::setText()` 需要 `QString`、列表行号从 0 开始、信号槽参数匹配。  
学到的技术：纯数据 `struct`、`QVector<Device>`、代码创建 QWidget、`currentRowChanged(int)` 新式连接、`QString::arg()` 文本格式化。  
下一步：加入 `QTimer` 成员和 `timeout` 连接，实现可控的模拟数据刷新；保留告警和日志到后续小步。

日期：2026-09-07  
功能：完成阶段 1 的 `QTimer` 定时模拟数据刷新。  
新增/修改文件：`include/mainwindow.h`（新增 `class QTimer;` 前置声明、`QTimer* refresh_timer_` 成员、`void refreshDeviceData();` 声明）、`src/mainwindow.cpp`（`#include <QTimer>`、`#include <QRandomGenerator>`、创建 `new QTimer(this)` + `connect(timeout→refreshDeviceData)` + `start(1000)`、实现 `refreshDeviceData()`）。  
构建命令：`cmake --build build-windows-qt -j2`。  
运行证据：程序启动后，选中某设备，右侧温度（20~60）、电压（1~5）每 1 秒在区间内随机变化并自动刷新详情。  
遇到的问题：`QListWidget` 行号从 0 起、未选中时 `currentRow()` 返回 `-1`，需先防越界；`QRandomGenerator::bounded(lowest, highest)` 右边界是开区间，电压要取 1~5 需写 `bounded(1, 6)`；`QTimer` 的源码端是定时器自身（`timeout` 由事件循环按周期触发，无需用户点击）；`QTimer::start(1000)` 传入毫秒并启动。  
学到的技术：`QTimer` 的 `timeout` 信号与 `start(interval)`；`QRandomGenerator::global()->bounded(lo, hi)`；信号槽的发送者可以是任何 QObject（定时器自触发）；`QListWidget::currentRow()`；数据（`Device`）与显示（`QLabel`）分离的刷新方式（改数据后再调 `updateDeviceDetails(row)`）。  
下一步：加入超限告警与日志记录（温度/电压超阈值时在 `log_view_` 写日志并给出告警提示）。

日期：2026-09-07  
功能：完成阶段 1 的「超限告警 + 日志记录」。  
新增/修改文件：`include/device.h`（新增 `bool alarm_` 字段）、`src/mainwindow.cpp`（新增 `constexpr double kMaxTemperature/kMaxVoltage` 阈值；`refreshDeviceData()` 中加入阈值判断与 `log_view_->append(...)` 日志写入）。  
构建命令：`cmake --build build-windows-qt -j2`。  
运行证据：选中设备，温度超 45 或电压超 4 时，日志区出现一条「告警:设备…温度…电压…」；读数回落出现「设备…恢复正常」；两者各只出现一次，持续超限不刷屏；右侧数值仍每 1 秒刷新。  
遇到的问题：`QString::arg` 占位符编号（`%4` 应为 `%3`，三个占位符配三个 `.arg`）；`#include <QRandomGenerator>` 后误多一个字母导致编译报错；`log_view_` 类型是 `QTextEdit`、写一行用 `append(QString)`；clang 的 `range-loop-detach` 与 `DeadStores`（`row` 未读）为警告/误报。  
学到的技术：给 `Device` 加告警状态字段的「数据对象设计」；`constexpr` 阈值常量放 `.cpp` 文件作用域；阈值比较用 `>`，用转折点逻辑（只在「正常→超限」「超限→正常」写一次日志）；`QTextEdit::append` 追加日志。  
下一步：阶段 1 完成。接下来进入阶段 2（配置与历史记录，用 `QJsonDocument`/`QFile` 读写配置和日志），或先 `git` 提交当前成果；仍未做串口、网络、线程、数据库或复杂图表。

日期：2026-09-07  
功能：完成阶段 2 的第一步「把设备列表保存为 JSON 配置文件」。  
新增/修改文件：`include/mainwindow.h`（新增 `class QPushButton;` 前置声明、`void saveConfig();` 声明、`QPushButton* save_button_` 成员）、`src/mainwindow.cpp`（新增 `<QPushButton>` `<QJsonArray>` `<QJsonObject>` `<QJsonDocument>` `<QFile>` include；`setupUi()` 中创建「保存配置」按钮 + `addRow` + `connect(clicked→saveConfig)`；实现 `saveConfig()`）。  
构建命令：`cmake --build build-windows-qt -j2`（或用 Qt Creator 构建）。  
运行证据：点击「保存配置」后，在工作目录（Qt Creator 运行时为其构建目录）生成 `config.json`，内容为含 3 台设备的 JSON 数组，每台 6 个字段（id/name/online/temperature/voltage/alarm），缩进整齐；保存的是内存当前快照（如 device03 因超限 `alarm=true`、device02 未刷新为初始值）。  
遇到的问题：`QFile::open` 不能对类名调用（需先 `QFile file(...)` 有对象，再用 `file.open(...)`）；保存用 `WriteOnly`（不是 `ReadOnly`）；`QByteArray` 是序列化出的字节、`QFile::write()` 需要它；`config.json` 相对路径落在"运行时工作目录"，Qt Creator 运行时是构建目录，需据此定位文件。  
学到的技术：Qt 布局自动管控件大小（`QFormLayout::addRow`）；`QPushButton` 代码创建 + `clicked` 信号；`QJsonObject`/`QJsonArray` 组装 + `QJsonDocument::toJson(QJsonDocument::Indented)` 序列化为 `QByteArray`；`QFile` 打开/写入/关闭 + 打开失败处理；对象树管理按钮生命周期；`const`/`constexpr` 与 JSON 数据结构概念。  
下一步：阶段 2 第二步——「读取配置」（用 `QJsonDocument::fromJson` + `QJsonArray`/`QJsonObject` 读回并填充设备列表），或先 `git` 提交当前成果。

日期：2026-09-07  
功能：完成阶段 2 的第二步「读取/恢复 JSON 配置文件」。  
新增/修改文件：`include/mainwindow.h`（新增 `void loadConfig();` 声明、`QPushButton* load_button_` 成员）、`src/mainwindow.cpp`（`setupUi()` 中新增「加载配置」按钮 + `addRow` + `connect(clicked→loadConfig)`；实现 `loadConfig()`）。  
构建命令：`cmake --build build-windows-qt -j2`（或用 Qt Creator 构建）。  
运行证据：点「保存配置」生成 `config.json` → 修改数据/等定时器变化 → 点「加载配置」后设备列表与右侧详情恢复成 `config.json` 里的样子；改坏/删除 `config.json` 再加载时日志区出现"打开配置失败"或"解析配置失败：…"，程序不崩溃。  
遇到的问题：`loadConfig` 里遍历的是 JSON 数组 `arr`（不是刚 `clear()` 后的 `devices_`，否则空转）；`QJsonArray` 元素是 `QJsonValue`，需 `.toObject()` 转成 `QJsonObject` 再取字段；键名必须与 `saveConfig` 一致；`QFile` 与 `QJsonDocument` 不是直接互转，需用 `QFile::readAll` 拿字节再 `QJsonDocument::fromJson`。  
学到的技术：`QJsonDocument::fromJson`（字节→文档）+ `QJsonParseError`（`isNull()`/`error`/`errorString()`）；`doc.array()` 取顶层数组；`QJsonValue.toObject()` 与 `.toString()/.toBool()/.toDouble()` 取值；数组/对象的区别与 JSON 根节点可为数组或对象；`QFile` 以 `ReadOnly` 读文件、`readAll` 取字节、`close`；「加载配置」用 `clear()` 替换而非追加。  
下一步：阶段 2 第二步完成。可继续「把运行日志写入文件」（用 `QFile` 追加），或先 `git` 提交当前成果；尚未做串口、网络、线程、数据库或复杂图表。

日期：2026-09-08  
功能：完成阶段 2 的收尾「把程序日志写入文件」。  
新增/修改文件：`include/mainwindow.h`（新增 `void writeLog(const QString& msg);` 声明）、`src/mainwindow.cpp`（新增 `#include <QDateTime>`；实现 `writeLog()`；把 `refreshDeviceData`/`saveConfig`/`loadConfig` 中共 5 处 `log_view_->append(...)` 改为 `writeLog(...)`）。  
构建命令：`cmake --build build-windows-qt -j2`（或用 Qt Creator 构建）。  
运行证据：`writeLog` 会把消息同时显示到 `log_view_` 并追加写入 `run.log`；`run.log` 每行带时间戳（如 `2026-09-08 00:06:59 告警:设备device01 温度41 电压5`）、行行分开、追加不覆盖旧内容。  
遇到的问题：`QFile::write` 不会自动换行，需末尾加 `"\n"`；`QString` 写文件需 `.toUtf8()` 转字节；打开失败应忽略（`if(file.open(...))` 内写），勿影响程序；`Append`（追加）而非 `WriteOnly`（否则每次运行覆盖）。  
学到的技术：`.log` 是普通文本日志文件；`QIODevice::Append|Text` 追加写；`QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")` 时间戳；抽 `writeLog()` 帮手统一"显示 + 落盘"，避免重复代码。  
下一步：阶段 2 全部完成（保存配置 / 读取配置 / 日志写文件）。下一步可进入阶段 3（Model/View 重构，用 `QAbstractListModel`/`QStandardItemModel` 管理设备列表），或先 `git` 提交当前成果；尚未做串口、网络、线程、数据库或复杂图表。

日期：2026-09-08  
功能：完成阶段 3 的第一步「用 `QAbstractListModel` 管理设备列表（Model/View 驱动）」。  
新增/修改文件：`include/DeviceListModel.h`、`src/DeviceListModel.cpp`（新建自定义 `DeviceListModel : QAbstractListModel`，持有 `QVector<Device>&` 引用，实现 `rowCount`/`data(DisplayRole)`，加 `reload()` 用 `beginResetModel/endResetModel` 通知视图）；`include/mainwindow.h`（`QListView* device_list_`、`DeviceListModel* device_model_`、前置声明）；`src/mainwindow.cpp`（`QListView` + `setModel`、选中信号 `QItemSelectionModel::currentChanged`、`loadConfig` 用 `reload()`+`setCurrentIndex`、`refreshDeviceData` 用 `selectionModel()->currentIndex().row()`）；`CMakeLists.txt`（加入 `src/DeviceListModel.cpp`、`include/DeviceListModel.h`）。  
构建命令：`cmake --build build-windows-qt -j2`（或用 Qt Creator 构建）。  
运行证据：编译通过；左侧设备列表由 `DeviceListModel` 提供（`rowCount`+`data`），显示 3 台设备名；点选设备右侧详情刷新；定时刷新/告警/保存/加载/日志照常。（待用户确认运行现象后视为验收。）  
遇到的问题：`QListView` 与 `QListWidget` API 不同（无 `currentRow`/`clear`/`addItem`/`setCurrentRow`），需改用 `selectionModel()`/模型 `reset`/`setCurrentIndex`；数据变化时须用 `beginResetModel/endResetModel` 通知视图；Qt Creator 新建类向导会把文件写成无 `src/`、`include/` 路径前缀，需手动修正。  
学到的技术：Model/View 分离（Model 提供数据、View 显示）；`QAbstractListModel` 实现 `rowCount`/`data(index, role)`；`Qt::DisplayRole`；`QModelIndex`；`QListView::setModel`；`QItemSelectionModel::currentChanged(QModelIndex, QModelIndex)` + lambda；`beginResetModel/endResetModel`；`setCurrentIndex(index, QItemSelectionModel::Select)`；`QVector` 引用共享数据。  
下一步：阶段 3 第一步完成（列表已模型驱动）。可进一步：让温度/电压也走 Model/View（用 `UserRole`/`EditRole` + `dataChanged` 刷新详情），或进入阶段 5（通信接入），或先 `git` 提交当前成果。

日期：2026-09-08  
功能：完成阶段 3 的深化「自定义数据角色 + `dataChanged` 通知视图」。  
新增/修改文件：`include/DeviceListModel.h`（新增 `enum DeviceRoles { NameRole/TemperatureRole/VoltageRole/OnlineRole = Qt::UserRole+1..+4 }`、`void deviceDataChanged(int row);` 声明）、`src/DeviceListModel.cpp`（`data()` 改为 `switch(role)`，`Qt::DisplayRole` 返回 `QStringLiteral("%1 温度%2")`，自定义角色返回对应字段；实现 `deviceDataChanged(row)` 用 `emit dataChanged(index(row,0), index(row,0))`）、`src/mainwindow.cpp`（`refreshDeviceData` 中改完数据后调用 `device_model_->deviceDataChanged(row)`）。  
构建命令：`cmake --build build-windows-qt -j2`（或用 Qt Creator 构建）。  
运行证据：左侧列表每行显示"设备名 温度xx"；选中设备后其温度每 1 秒变化，列表对应行随之更新（`dataChanged` 生效）；右侧详情刷新、告警/保存/加载/日志照常。  
遇到的问题：`data()` 若只处理自定义角色而漏掉 `Qt::DisplayRole`，`QListView` 会用 `Qt::DisplayRole`(0) 取值而落入 `default` 返回空 → 列表空白；`QString` 字符串字面量应优先 `QStringLiteral`（编译期构造，无运行期转换）。  
学到的技术：自定义 `Qt::ItemDataRole`（基于 `Qt::UserRole` 起）；`data(index, role)` 按角色返回不同字段；`emit dataChanged(topLeft, bottomRight)` 通知视图某行数据已变（细粒度刷新，区别于 `reload()` 的整体 reset）；`role` 决定取哪个字段、`index` 决定哪一行；`switch/case` 与 `QString::arg` 格式化。  
下一步：阶段 3（Model/View）已完成（列表模型驱动 + 自定义角色 + dataChanged）。下一步可进入阶段 4（实时曲线）或阶段 5（通信接入），或先 `git` 提交当前成果。

日期：2026-09-08  
功能：完成阶段 4 的第一版「用 QPainter 自绘温度历史折线」。  
新增/修改文件：`include/TempChartWidget.h`、`src/TempChartWidget.cpp`（新建 `TempChartWidget : QWidget`，`QVector<double> points_` + `static constexpr int kMaxPoints = 60`；`addValue` 追加并 `removeFirst` 滚动限长后 `update()`；重写 `paintEvent` 用 `QPainter` 画背景、求 min/max（防除零）、把 `(i, points_[i])` 映射为像素、`drawPolyline` 画折线；`clear()`）；`include/mainwindow.h`（前置声明 + `TempChartWidget* temp_chart_`）、`src/mainwindow.cpp`（`#include "TempChartWidget.h"`；`setupUi` 中建"温度曲线"分组并加入 `root_layout`；`refreshDeviceData` 中 `temp_chart_->addValue(device.temperature_)`）、`CMakeLists.txt`（加入 `src/TempChartWidget.cpp`、`include/TempChartWidget.h`）。  
构建命令：`cmake --build build-windows-qt -j2`（或用 Qt Creator 构建）。  
运行证据：窗口自上而下为「设备/详情分栏 → 温度曲线 → 运行日志」；"温度曲线"面板出现蓝色折线，随选中设备温度每秒变化而延伸；点数据攒够后铺满宽度并向左滚动。  
遇到的问题：用了 `QPainter` 对象却未 `#include <QPainter>` → "incomplete type"；`class QWidget;` 前置声明不能用作基类（继承需完整定义，要 `#include <QWidget>`）；定义里重复写默认参数 `= nullptr` → "Redefinition of default argument"；`y` 误用索引 `i` 而非温度值 `points_[i]`（画不出温度曲线）；**只声明 `temp_chart_` 未 `new` 就使用 → 空指针崩溃**（必须"先创建后用"）；同一控件被加入布局两次。  
学到的技术：自定义 `QWidget` + 重写 `paintEvent(QPaintEvent*)` + `QPainter`；`update()` 请求重绘；数据坐标→像素坐标映射（`x=i/(kMaxPoints-1)*宽`、`y=高-(值-min)/(max-min)*高`）；滚动窗口（`removeFirst`）控制数据点；"对象/继承需要 include 完整定义，指针/引用才可用前置声明"；`QPolygonF`/`QPointF` 与 `<<` 追加、`drawPolyline`。  
下一步：阶段 4 第一版完成。可选增强：给曲线加坐标轴/网格/数值刻度，或按设备分别记录历史（现只记当前选中设备），或进入阶段 5（通信接入）。

日期：2026-09-11  
功能：完成阶段 4 增强「温度曲线加坐标轴 + 网格线 + 数值刻度」。  
新增/修改文件：`src/TempChartWidget.cpp`（`paintEvent` 重写：新增 `<QRectF>`/`<QColor>` include；用边距切出绘图区 `QRectF plot`；固定刻度 `kMinTemp=20/kMaxTemp=60/kStep=10`；循环画浅灰水平网格线 `drawLine` + 左侧刻度数字 `drawText(QRectF, AlignRight|AlignVCenter, QString::number(int(v)))`；画 Y 轴竖线与 X 轴横线；曲线映射由"整个控件 + 数据 min/max"改为"基于 `plot` + 固定范围"，删除旧的 min/max 与防除零代码；`clear()` 补 `update()`；`QPoint` 改为 `QPointF`）。  
构建命令：`cmake --build build-windows-qt -j2`（或用 Qt Creator 构建）。  
运行证据：温度曲线面板出现 Y 轴刻度 20/30/40/50/60、5 条浅灰网格线、左竖+底横两条轴；蓝色曲线画在绘图区（左侧/底部留白），随温度每秒更新；曲线与对应刻度线对齐。  
遇到的问题：`QRectF(x,y,宽,高)` 的第二个参数是 y（上偏移），误写成 `bottomMargin`；x 误用整个 `width()` 而非 `plot.width()`（曲线溢出右边界）；y 公式漏掉 `(points_[i] - kMinTemp)` 且用整个 `height()`（高温画到控件外）；用 `QPoint`（整数）代替 `QPointF` 导致坐标被截断；`clear()` 漏 `update()`；网格与曲线的 y 必须用**同一公式**才能对齐。  
学到的技术：`QRectF` 表示"绘图区"，由控件尺寸减边距得到；数据坐标→像素坐标映射（x 按索引、y 按值，且 y 需反转）；`QPainter::drawLine/drawText`、`QPen`+`QColor` 区分网格（浅灰细）与曲线（蓝粗）；`Qt::AlignRight|Qt::AlignVCenter` 放置刻度数字；固定刻度范围（分母恒定，免防除零）。  
下一步：阶段 4 增强完成。可选：按设备分别记录历史、电压曲线、X 轴时间刻度；或进入阶段 5（通信接入）。

日期：2026-09-18（实际跨到 9-19 凌晨完成）
功能：完成阶段 5 第一步「`Transport` 抽象接口 + `TcpTransport` 实现」，打通 TCP 客户端收发链路，达成 Day 2 全部四条验收。
新增/修改文件：
- `include/Transport.h`：在既有抽象接口上新增两个信号 `void closed();` 和 `void sendFailed(const QString& reason);`（判断依据：连接断开、发送失败是 TCP/串口/回环**共有**的能力，属于抽象层，不属于某一种传输方式）。
- `include/TcpTransport.h`：新增 `void setHost(const QString& host);` / `void setPort(quint16 port);` 声明；新增成员 `QString host_ = "127.0.0.1";` / `quint16 port_ = 8888;`。
- `src/TcpTransport.cpp`：构造函数 `socket_ = new QTcpSocket(this);` + 4 个 `connect`（`connected→openSucceeded`、`disconnected→closed`、`errorOccurred→openError(errorString())`、`readyRead→receiveByte(readAll())`）；实现 `open()`（判 `state()` 后 `connectToHost(host_, port_)`）、`close()`（`disconnectFromHost()`）、`isConnected()`（判 `ConnectedState`）、`sendByte()`（先拦未连接 → `emit sendFailed` → `write(chunk)`）、`setHost()`/`setPort()`。
- `include/mainwindow.h`：新增 `class TcpTransport;` 前置声明、`TcpTransport* transport_ = nullptr;`、`QPushButton* connect_button_ = nullptr;`。
- `src/mainwindow.cpp`：新增 `#include "TcpTransport.h"`；创建 `transport_ = new TcpTransport(this);` 并把 5 个信号（`openSucceeded`/`openError`/`sendFailed`/`closed`/`receiveByte`）以 `&Transport::` 限定连到 `writeLog()`；新增「tcp连接」按钮，点击调 `transport_->open()` 并把返回值写进日志；临时注释 `sim_timer->start(800);`（假数据源退场，避免淹没真实日志）。
- `CMakeLists.txt`：新增 `target_compile_options(device_monitor PRIVATE -Wall -Wextra)`。
- 新增 `tools/build.ps1`（一键构建，锁死 Qt 自带 MinGW + 正确的 `-D` 引号写法）、`tools/tcp_feed.ps1`（本机 TCP 回环数据源，逐步打印状态）。
- `DAILY_CARDS.md`：顶部补「构建方式（2026-09-19 更新）」一节。

构建命令：`powershell -ExecutionPolicy Bypass -File tools\build.ps1`（Qt Creator 亦可）。0 error、0 warning。

运行证据：先用 `powershell -ExecutionPolicy Bypass -File tools\tcp_feed.ps1` 在 `127.0.0.1:8888` 起监听；点「tcp连接」后，日志区依次出现
`已受理` → `连接成功` → `接收成功，数据为:{"id":"1","temperature":37,"voltage":3}`；
监听端 15 秒后关闭连接，日志出现 `连接已断开`，**程序不崩溃**；**再次点击可再次连接成功**（第二轮完整重放）；连接无人监听的端口时出现 `连接失败:Connection refused`（明确报错，非静默失败）。监听端打印 `[4] sent #1 -> 40 bytes`，与 payload 39 字节 + `\n` 完全一致，说明零丢字节。

遇到的问题：
- **环境（本日主要耗时项）**：① 仓库改名导致三个 CMake 构建目录因缓存内写死旧绝对路径而全部失效（见文首说明）；② PATH 上是 MSYS2 的 GCC 15.2.0，与 Qt 6.11.0 mingw_64 的 GCC 13.1.0 ABI 不匹配；③ **PowerShell 5.1 对以 `-` 开头的原生命令参数不做变量展开**，`-DCMAKE_PREFIX_PATH=$QtDir` 会原样传给 cmake，报出来的却是「找不到 Qt6」——错误位置与真实原因不在一起（加双引号写成 `"-DCMAKE_PREFIX_PATH=$QtDir"` 即可）。
- **代码**：`emit close();` —— `emit` 是空宏，该行实际是**调用虚函数 `close()`**，`closed()` 信号从未发出，且**编译零警告**（`close` 与 `closed` 一字之差）；`socket_ != nullptr` 不能当连接判据（构造函数已 `new`，终生非空）；`transport_ = new QTcpSocket(this)` 类型错——把实现当成了抽象，若成立会让 `MainWindow` 直接耦合 TCP、使整个 `TcpTransport` 沦为死代码；`receiveByte` 最初无人 `connect`，数据进黑洞；`open()` 初版漏写 `return true`（"not all control paths return a value"，路径无返回值属未定义行为）。
- **工具**：多行脚本粘贴进 PowerShell 控制台不可靠（行被截断或提前提交），脚本只执行了一部分——表现为"客户端已接入但一个字节都没发"。改用 `.ps1` 文件运行后排除。

学到的技术：`QTcpSocket` 的非阻塞异步模型（`connectToHost()` 立即返回，结果只走 `connected`/`errorOccurred` 信号）；`QAbstractSocket::SocketState` 状态判据（`open()` 只放行 `UnconnectedState`，`isConnected()` 只认 `ConnectedState`——`ConnectingState` 不算已连接）；`readyRead` 中一次 `readAll()` 读空（它是"有新数据"而非"有一条完整消息"）；`errorString()` 与 `QString::fromUtf8()` 解码；`disconnectFromHost()` 异步优雅关闭 vs `abort()`，以及为何**不能在事件循环里阻塞等待**异步完成（死锁）；`QStringLiteral`（编译期构造）；`&Transport::信号` 限定——**抽象层的设计在这里兑现**：`MainWindow` 只依赖 `Transport`，Day 11 加串口时它一行都不用改；`HostLookupState` 只在主机名为域名时出现（写 IP 字面量则跳过，`localhost` 还会引入 IPv6 `::1` 优先的不确定性）。

下一步：进入阶段 5 第二步，按 Day 4 卡片做 **M1** —— 收发框 `DataView`（文本 / HEX 双模式、`rx`/`tx` 字节计数、最大行数限制），并让曲线改由**解析后的真实数据**驱动（`receiveByte` → `JsonLineParser` → 曲线），正式决定 `sim_timer` 的去留。另有一处待收口：`errorOccurred` 在对端**正常关闭**时也会触发 `RemoteHostClosedError`，目前会多打一条「连接失败」，应过滤或改为中性文案。



## 记录格式



每完成一个功能，在本文件追加：

```text
日期：
功能：
新增/修改文件：
构建命令：
运行证据：
遇到的问题：
学到的技术：
下一步：
```

## 个人 Qt 速查表（需长期维护）

- 文件：`docs/qt_cheatsheet.md`。
- **每学会一个新的 Qt 类/函数，就往该文件加一行**（"任务 → 类 → 关键函数"）；踩过的坑也追加到它的"坑"一节。
- 记不住 API 时**先查该文件**，再用 Qt Creator 的 `Ctrl+Space` / `F1` 确认细节——**不要试图背 API**。
- 新会话开始时，除了本文件，也可一并参考 `docs/qt_cheatsheet.md`。
