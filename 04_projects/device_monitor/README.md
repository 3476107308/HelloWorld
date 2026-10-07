# device_monitor

> **面向 Qt/C++ 上位机方向的串口 / TCP 调试与实时监控工具**
> Qt 6 + CMake 独立实现。功能组织参考 SerialTest，**未复制其源码**。

---

## 一句话亮点

**多线程收发 + UI 不卡顿。**

100 KB/s 持续灌入时，网络 I/O 全部在独立工作线程完成，**主线程 CPU 占用 0.1%**，界面全程可交互（切换 HEX、输入文字、拖动分栏、曲线实时刷新、日志持续滚动）。

这不是形容词 —— 下面有**实测数据**、**线程身份证据**和**可复现的测试脚本**。

---

## 功能

| 模块 | 能力 |
|---|---|
| **连接管理** | TCP 连接 / 断开；主机端口校验（1–65535）；四态状态灯（未连接 / 连接中 / 已连接 / 错误）；失败时显示具体原因 |
| **收发显示** | 文本 / HEX 双模式；rx / tx 字节计数；显示区限长 500 行 |
| **数据发送** | UTF-8 文本发送；可勾选追加 `\r\n` |
| **协议解析** | 文本 JSON、一行一条（`\n` 分隔）；缓冲区 + 循环切分处理**粘包 / 半包**；解析失败丢一条并记日志，不影响后续 |
| **实时曲线** | `QPainter` 自绘温度折线；Y 轴刻度 + 网格线；范围随数据自适应；绘图区裁剪 |
| **CSV 导出** | `QFileDialog` 选路径、`QTextStream` 逐行写、CRLF 行尾（Excel / 记事本兼容）；含「取消」与「无数据」两条边界 |
| **运行日志** | 界面日志区 + `run.log` 文件双写，带时间戳；日志区限长 500 行 |

---

## 性能实测

| 指标 | 实测值 | 测量方式 |
|---|---|---|
| **持续吞吐** | **100.0 KB/s** | `tools/tcp_stress.ps1`（1024 B × 100 条/秒），连续 2 轮 × 25 s |
| **发送端背压** | **零** —— 全程 99.4–100.0 KB/s，一次未回落 | 发送端每秒自报速率 |
| **主线程 CPU** | **0.1%** | 任务管理器，灌数据期间 |
| **进程内存** | **49 MB** | 任务管理器（优化前 269 MB） |
| **界面可交互性** | HEX 切换、文本输入、曲线刷新、日志滚动 **全部流畅** | 灌数据期间人工操作 |
| **线程身份** | UI `QThread(0x…362e0, "Qt mainThread")` ≠ io `QThread(0x…97d60)` | `qDebug() << QThread::currentThread()` |

> **「零背压」比速率本身更重要**：发送端从未被 TCP 缓冲区拖慢，说明应用**完全跟得上** 100 KB/s —— 瓶颈既不在网络层，也不在 I/O 线程。

### 一次真实的性能优化

压测发现：**横向拖动分栏线（改变宽度）明显卡顿**，其余操作全部流畅。

| 阶段 | 内容 |
|---|---|
| **对照实验定位** | 同一根把手：文档为空时流畅 → 灌完 1 万行日志后迟钝 → 清空后又流畅 → 锁定根因是**文本重新折行排版**，与网络和数据量无关 |
| **根因** | 日志区用了 `QTextEdit`（富文本模型，**没有** `setMaximumBlockCount`），且每条报文把 1030 字符原始字节抄进日志 → 文档涨到 **5.4 MB / 10673 行** → 每次改变宽度都要全文重排 |
| **修复** | `QTextEdit` → `QPlainTextEdit` + `setMaximumBlockCount(500)`；逐包原文不再入日志（收发框已显示原始字节） |
| **效果** | 日志文本量 **↓25 倍**，进程内存 **269 MB → 49 MB**，卡顿消失，吞吐无回退 |

---

## 架构

### 模块职责

| 模块 | 职责 |
|---|---|
| `Transport`（抽象） | 传输层接口：`open/close/isConnected/sendByte` + 5 个信号，**不含任何具体实现** |
| `TcpTransport` | `Transport` 的 TCP 实现，封装 `QTcpSocket` |
| `IoWorker` | 传输层工作对象：持有 `Transport`，向 UI 暴露 `dataReceived` 等信号 |
| `MainWindow` | **唯一的中枢**：所有信号槽在此汇合，也是唯一被允许操作 UI 的对象 |
| `DataView` / `ConnectBar` / `TempChartWidget` | 纯 UI 控件，只发信号，不碰业务 |
| `JsonLineParser` | 协议解析：缓冲区 + 按 `\n` 循环切分 |

**两条设计原则**：

1. **同层组件不互相认识** —— 通信一律经过 `MainWindow`
2. **每一层只认识自己的下一层** —— `MainWindow` 认识 `IoWorker`，`IoWorker` 认识 `Transport`，而 **`MainWindow` 完全不认识 `Transport`**

### 线程模型

```
主线程 (UI)                              工作线程 (io)
─────────────                            ─────────────
MainWindow ── emit requestOpen ──▶ [队列] ──▶ IoWorker::openConnection
                                                    │
                                             TcpTransport::open()
                                                    │
                                              QTcpSocket 读写（阻塞 I/O）
                                                    │
UI 更新 ◀── [队列] ◀── emit dataReceived ── IoWorker
```

- `IoWorker` 用 `moveToThread()` 移入独立 `QThread` —— **必须无 parent**，否则搬家失败
- `TcpTransport` 在**工作线程内**创建，保证 socket 与线程归属一致
- 跨线程连接自动使用 `Qt::QueuedConnection`，参数被拷贝后投递
- 线程启动用 `connect(io_thread_, &QThread::started, worker_, &IoWorker::start)`，**不直接调用 `start()`**
- 退出序列：`quit()`（异步请求）→ `wait()`（阻塞等待）→ `delete worker_`

详见 `docs/architecture.md`。

---

## 构建与运行

**环境**：Windows + Qt 6.11.0 (MinGW 64-bit, GCC 13.1.0) + CMake ≥ 3.19 + Ninja

```powershell
cd 04_projects\device_monitor
powershell -NoProfile -ExecutionPolicy Bypass -File tools\build.ps1
```

> `tools/build.ps1` 会把 Qt 自带的 MinGW 13.1.0 前置到 PATH。系统 PATH 上的其他 GCC（如 MSYS2 的 GCC 15）与 Qt 的 ABI 不匹配，会产生难以定位的链接错误。
>
> 也可以用 Qt Creator 打开 `CMakeLists.txt`（Kit：`Desktop_Qt_6_11_0_MinGW_64_bit-Debug`）。

---

## 可复现的测试场景

三个脚本，全部在 `tools/` 下：

```powershell
# ① 基础数据源：监听 8888，客户端接入后发 N 条 JSON
powershell -ExecutionPolicy Bypass -File tools\tcp_feed.ps1 -Port 8888 -Count 100 -IntervalMs 50 -RandomTemp

# ② 吞吐压测：稳定 100 KB/s（加 -Fast 则打极限）
powershell -ExecutionPolicy Bypass -File tools\tcp_stress.ps1 -Port 8888 -Kbps 100 -Seconds 25

# ③ 粘包 / 半包测试：一次连接跑完 5 个用例
powershell -ExecutionPolicy Bypass -File tools\tcp_framing_test.ps1 -Port 8888
```

**先跑脚本**（它开始监听），**再启动程序并点「连接」**。

### 粘包 / 半包测试用例与结果

| # | 用例 | 网线上做了什么 | 预期 | 实测 |
|---|---|---|---|---|
| 1 | **SPLIT**（半包） | 一条 JSON 切成两段，间隔 600 ms | 1 条 | ✅ 1 条 |
| 2 | **STICKY**（粘包） | 3 条 JSON 挤进**一次** `Write()` | 3 条，顺序正确 | ✅ 3 条 |
| 3 | **BYTE-WISE** | 一条 JSON 拆成 40 次**单字节**写 | 1 条，且只在 `\n` 到齐后出现 | ✅ 1 条 |
| 4 | **MIXED** | 一次写 = 上条尾部 + 完整一条 + 下条头部 | 3 条，顺序正确 | ✅ 3 条 |
| 5 | **RECOVERY** | 先发一行非法文本，再发一条合法 JSON | 1 条解析失败 + 1 条正常 | ✅ 各 1 条 |

**总账：9 条消息零丢失、零乱序，1 条容错 —— 与发送端逐条对账一致。**

> 测试脚本本身也被验证过：第一版 CASE 4 多打了一个引号，导致 41 被解析成错误行；靠逐条对账才发现并修正。**测试也要被验证。**

### CSV 导出验收

| 检查项 | 结果 |
|---|---|
| 行数 | **101** 行 = 1 表头 + 100 数据 |
| 文件大小 | **3834 字节** = (32+2) + 100×(36+2) → **逐字节证明每行都是纯 CRLF**，既没退化成 LF，也没变成 `\r\r\n` |
| 最后一行 | 真数据，非空 → `QTextStream` 缓冲未丢尾部 |
| 边界 | 「取消」→ 记日志、不生成文件；「无数据」→ 记日志、不弹对话框 |

---

## 已知限制

- **只实现 `Line` 分包模式（`\n` 分隔）**，未做 `LengthPrefix` / 定长模式
- **未接真实串口硬件**：本机 Qt 未安装 SerialPort 模块且无硬件。`Transport` 抽象已为 `SerialTransport` 预留接口，补齐时 **`MainWindow` 无需改动**
- **CSV 未做字段转义**：id 含逗号时列会错位（当前 id 为数字，不触发）
- **曲线只画温度**，未画电压；X 轴无时间刻度
- **曲线 Y 轴范围跟随数据自适应**：超范围数据显示正确，但刻度会随数据跳动
- **`tx` 计数是乐观的**：`sendByte` 失败时仍会累加

---

## 目录结构

```
device_monitor/
├── include/     头文件（Transport / TcpTransport / IoWorker / MainWindow / ...）
├── src/         实现
├── tools/       构建与测试脚本（build / tcp_feed / tcp_stress / tcp_framing_test）
├── docs/        架构说明 / 协议草案 / Qt 速查表 / 面试问答
├── CMakeLists.txt
└── README.md
```

---

## 文档

| 文档 | 内容 |
|---|---|
| `docs/architecture.md` | 模块职责表、线程模型详解 |
| `docs/protocol.md` | 通信协议草案（JSON + `\n`、字段定义、粘包半包策略） |
| `docs/interview_qa.md` | 6 个面试问题的完整回答 |
| `docs/qt_cheatsheet.md` | 「任务 → 类 → 关键函数」速查表 + 踩坑清单 |

---

## 运行截图

> **待补**：连接成功 + 曲线绘制 + 日志滚动的整窗截图

---

## 附：早期开发过程记录（学习日志存档）

> 以下是项目早期的阶段化学习记录（9/7 – 9/11），保留作过程存档。
> **当前项目状态以上方 README 为准。**

## 学习规则

1. 每次只实现一个可运行功能，不一次生成整套项目。
2. 新会话开始先阅读本文件、`SESSION_PROMPT.md`、`docs/architecture.md` 和当前 Git 状态。
3. 每个功能都要经过：需求确认、技术门槛检查、用户独立编码、构建运行、代码审查、复盘记录。
4. 练习代码和关键逻辑优先由用户完成。助手负责拆任务、指出错误、解释原因，不直接替用户写完全部功能。
5. 不重复学习已经通过运行验证的 Qt 信号槽、基础 QWidget、CMake 和 C++ 基础；只在项目遇到具体问题时补精准知识。
6. 不复制 `SerialTest` 的 GPL 源码。只参考公开的功能划分、交互流程和模块边界；正式项目代码独立编写。
7. **维护 `docs/qt_cheatsheet.md`**：每学会一个新的 Qt 类/函数，就往里加一行（"任务 → 类 → 关键函数"）。记不住 API 时先查它，再用 Qt Creator 的 `Ctrl+Space` / `F1` 确认细节；**不要试图背 API**。踩过的坑也追加到该文件的"坑"一节。

## 当前技术基线

- C++：已掌握基础语法、OOP、STL、RAII、智能指针、lambda、C++11 常用特性。
- CMake：已能构建多文件项目和静态库，Week06 Day04/Day05 的 Ubuntu 验收记录仍需补齐。
- Qt：已完成 QWidget、Designer、标准信号槽、自定义信号槽、lambda 接收、QMessageBox。
- 环境：Windows + Qt 6.11 MinGW 64-bit；后续需要 Linux 构建时使用独立的 `build-linux/`。

## 功能路线

### 阶段 0：工程骨架

- Qt6 + CMake 工程能够配置、构建、运行。
- 主窗口、左侧设备列表、右侧详情区和底部日志区。
- 验收：能运行；关闭窗口正常退出；源码按 `include/`、`src/`、`ui/` 或等价模块分离。

### 阶段 1：模拟设备数据（当前起点）

- `Device` 数据结构：设备名、在线状态、温度、电压、更新时间。
- 使用 `QTimer` 产生可控的模拟数据。
- 设备列表点击后更新详情区。
- 超限时显示告警并写入日志。
- 验收：至少 3 个设备；数据定时变化；选择设备后详情同步；告警可复现。

当前进度（2026-09-07）：

- 已完成 `Device` 纯数据结构和 `QVector<Device>` 设备容器。
- 已完成设备名称从数据容器填充到 `QListWidget`。
- 已完成 `currentRowChanged(int)` 选择信号连接。
- 已完成右侧设备名称、在线状态、温度和电压详情更新。
- 已完成 `QTimer` 定时模拟数据刷新：每 1 秒让选中设备的温度（20~60）、电压（1~5）在区间内随机变化并自动刷新详情。
- 已完成超限告警与日志记录：选中设备温度 > 45 或电压 > 4 时在日志区写一条「告警」，读数回落时写一条「恢复正常」，各只出现一次，持续超限不刷屏。

进入条件：阶段 0 已构建运行，且能解释 `QObject` 父子关系、信号槽连接和 `QTimer::timeout`。

### 阶段 2：配置和历史记录

- 用 `QJsonDocument`/`QJsonObject` 保存设备配置。
- 用 `QFile` 读写配置和日志。
- 增加“加载默认配置、保存配置、恢复配置”。

需要补的技术：Qt JSON、Qt 文件 I/O、错误处理和路径选择。只补这些，不重学 Qt 基础。

当前进度（2026-09-08）：

- 已完成「把设备列表保存为 JSON 配置文件」：窗口详情区新增「保存配置」按钮，点击用 `QJsonObject/QJsonArray/QJsonDocument::toJson` + `QFile` 把 `devices_` 写入 `config.json`。
- 已完成「读取/恢复 JSON 配置文件」：新增「加载配置」按钮，点击用 `QFile::readAll` + `QJsonDocument::fromJson` + `QJsonArray/QJsonObject` 读回并重新填充 `devices_` 与列表；解析/打开失败会提示且不崩溃。
- 已完成「把程序日志写入文件」：`writeLog(msg)` 把消息同时显示到 `log_view_` 并追加写入 `run.log`（带时间戳、每行一条、追加不清空）；原 `log_view_->append` 调用点已改用 `writeLog`。

阶段 2（配置和历史记录）已完成。

### 阶段 3：Model/View 重构

- 用 `QAbstractListModel` 或 `QStandardItemModel` 管理设备列表。
- 让数据层与界面层分离，避免把设备数据直接塞进窗口控件。

需要补的技术：Model/View 的数据角色、索引、`dataChanged`、`beginResetModel/endResetModel`。

当前进度（2026-09-08）：

- 已完成第一步：新建 `DeviceListModel : QAbstractListModel`（持有 `QVector<Device>&`，实现 `rowCount`/`data(DisplayRole)`，加 `reload()`），把左侧列表从 `QListWidget` 换成 `QListView` + `setModel`；选中变化用 `QItemSelectionModel::currentChanged`，`loadConfig` 用 `reload()` 刷新、用 `setCurrentIndex` 选中首行。
- 已完成深化：给模型加自定义角色（`TemperatureRole`/`VoltageRole`/`OnlineRole`），`data()` 按角色返回不同字段；`data()` 的 `Qt::DisplayRole` 显示"设备名 温度xx"；定时刷新改数据后用 `device_model_->deviceDataChanged(row)` 发 `dataChanged` 通知视图，列表那一行随温度更新。

阶段 3（Model/View 重构）已完成。

### 阶段 4：实时曲线

- 为选中设备展示温度/电压历史曲线。
- 先使用已有 Qt 能力或一个明确的 Qt6 兼容绘图库，控制数据点数量。

需要补的技术：绘图控件、定时刷新、历史数据窗口和性能边界。没有确认库和构建方式前，不先引入第三方依赖。

当前进度（2026-09-11）：

- 已完成第一版：新增 `TempChartWidget : QWidget`（用 `QPainter` 在 `paintEvent` 自绘温度历史折线，`QVector<double>` 存历史，`kMaxPoints=60` 滚动限长）。窗口在「分栏」与「日志」之间新增"温度曲线"面板；`refreshDeviceData` 中把当前选中设备的温度喂给曲线。
- 已完成增强：曲线带 **Y 轴刻度（20~60，每 10 一格）+ 浅灰水平网格线 + 两条轴**；用边距切出绘图区 `QRectF plot`，曲线与网格用同一套坐标映射（基于 `plot` + 固定范围），因此曲线与刻度对齐。
- 待做的可选增强：按设备分别记录历史（现只记当前选中设备）；电压曲线；X 轴时间刻度。

### 阶段 5：通信接入

- 先接入 `QSerialPort`，让模拟数据和真实串口数据共用同一数据接口。
- 再按需求增加 `QTcpSocket` 或 UDP。

需要补的技术：串口参数、异步 I/O、`readyRead`、缓冲区、消息边界、错误和断线处理。进入本阶段前必须先完成通信协议草案。

当前进度（2026-09-11）：

- ✅ **通信协议草案已完成**：`docs/protocol.md`（文本 JSON + `\n` 分隔 + `QJsonDocument::Compact` 序列化；字段 `id`/`temperature`/`voltage` 必填，`online`/`ts` 可选；缓冲区 + 按 `\n` 循环切分解决粘包/半包；解析失败丢弃并继续）。
- 待做：实现通信接入。**因当前无串口硬件**，先做「模拟数据源 + 协议解析器」把"边界/粘包半包/容错"跑通（不依赖硬件与 Qt SerialPort 模块），再以 `QSerialPort` 实现同一接口接真设备。

### 阶段 6：线程与工程化

- 只有出现阻塞 I/O、解析耗时或 UI 卡顿证据时才引入线程。
- 增加单元测试、日志级别、README 截图、构建说明和可复现测试场景。

需要补的技术：QObject 线程归属、queued connection、线程安全、生命周期和测试框架。没有实际并发需求不提前学习。

## 每个功能的固定协作格式

助手开始新功能时必须先说明：

- 这次只实现什么。
- 会新增或修改哪些文件。
- 用户已经具备哪些前置知识。
- 当前缺口是什么，是否需要先补技术栈。
- 验收命令、运行现象和失败时的排查顺序。

用户完成后应提供：

- 修改后的文件或关键 diff。
- CMake 配置/构建输出。
- 程序运行现象或截图。
- 自己对关键设计的两三句话解释。

## 当前下一步

阶段 0–3 已完成（工程骨架、模拟数据 + 告警 + 日志、配置读写 + 日志文件、Model/View 重构），阶段 4 第一版完成（`QPainter` 温度历史折线）。剩余：

- **阶段 4 可选增强**：坐标轴/网格/数值刻度；按设备分别记录历史（现只记当前选中设备）；电压曲线。
- **阶段 5 通信接入（优先）**：先写通信协议草案，再接入 `QSerialPort`（串口参数、异步 I/O、`readyRead`、缓冲区、粘包/半包、错误与断线处理），让模拟数据与真实数据共用同一更新接口；再按需加 `QTcpSocket`/UDP。
- **阶段 6 线程与工程化**：只在出现阻塞/卡顿证据时引入线程；补单元测试、日志级别、README 截图、构建说明、可复现测试场景、Release 打包。
- **可展示收尾**：README（环境/构建/运行/截图/功能/已知问题）、演示截图或 GIF。
