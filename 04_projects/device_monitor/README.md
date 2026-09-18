# device_monitor

一个面向 Qt/C++ 上位机方向的设备监控面板。项目参考 `sandbox/reference_projects/SerialTest` 的功能组织方式，但使用 Qt 6 + CMake 独立实现，不复制参考仓库源码。

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
