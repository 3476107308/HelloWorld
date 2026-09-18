# device_monitor 架构记录

## 当前状态

- 阶段 0 工程骨架已创建，源码可以用 Qt 6.11.0 + MinGW 13.1.0 + CMake/Ninja 配置并编译。
- 参考样本：`D:\AAA study\sandbox\reference_projects\SerialTest`。
- 技术选择：Qt 6 + CMake；正式项目不使用 qmake。

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
