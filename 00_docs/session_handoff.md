# 会话交接文档

> 新会话开始时请先阅读本文件，再查看 `00_docs/trial_8_week.md`、`00_docs/roadmap.md` 和当前 Git 状态。

## 1. 我们正在做什么

用户是南通大学计算机科学与技术专业学生，当前大一升大二，目标是本科毕业后直接就业，毕业目标年薪 20 万以上。短期第一优先级是争取 2027 年寒假获得第一段实习，实习薪资要求不高，只要基本支持生活成本即可。

当前采用的职业路线是：

> 短期主线：Qt/C++ 客户端、工业软件、上位机和 Linux C++。
> 长期方向：根据实践结果，逐步向嵌入式 Linux 或机器人软件发展。

不应在寒假前把全部时间押在嵌入式 Linux 驱动或 ROS/视觉算法上，因为用户目前没有开发板，也没有真实硬件或 ROS 项目经验；这些方向保留为后续发展方向。

## 2. 用户已有基础

- C++：面向对象、STL、C++11 部分特性、智能指针、RAII、右值引用和移动语义，已完成前 5 周大部分练习。
- 数据结构：基础；获得蓝桥杯 C++ B 组省二。
- CMake：会最基本的三条命令进行配置、编译，但工程化能力还需要加强。
- Linux：使用 VMware 中的 Ubuntu；会基本命令，对进程调度了解一点，尚未掌握设备树、驱动等深入内容。
- Git：会基本命令和处理冲突，深入使用能力有限。
- Qt：基础。
- OpenCV：上学期学过，但缺少实际项目经验。
- 项目：C++/EasyX 坦克大战，主要负责排行榜的文件读写部分；未负责寻路和碰撞检测。
- RoboMaster：视觉组成员，暂时没有完成可独立展示的具体任务。
- 每周可稳定投入：15 小时以上。
- 成绩：年级排名约前 15%–25%，无挂科，英语四级尚未通过。
- 地域：优先长三角，但城市没有硬性限制；无锡是家乡。
- 暂无开发板；后续预算为 100–300 元。

## 3. 已经完成的工作

### 学习内容

用户大致完成了第 1–5 周：

1. 指针、引用、const、内存和五法则。
2. 继承、多态、虚函数和虚析构。
3. STL、学生信息管理系统。
4. 模板、智能指针、RAII。
5. GDB 基础、`auto`、`decltype`、`nullptr`、右值引用、`std::move`、`std::forward`。

代码中仍存在一些低级错误和工程问题，因此不能把前 5 周视为完全掌握；后续应通过修复、测试、解释和项目应用巩固，而不是重新从零学习全部 C++。

### 仓库整理

仓库根目录为 `D:\AAA study`，已整理为：

```text
00_docs/                  计划、Prompt、交接文档、进度
01_cpp_foundation/       C++ 第 1–6 周
02_linux_network/        Linux、进程、线程、网络
03_qt/                   Qt 学习
04_projects/             可写进简历的完整项目
05_embedded/             STM32、串口、RTOS 等实验
sandbox/                 临时实验
99_archive/              旧版本和废弃内容
```

已完成的移动：

- `week01` -> `01_cpp_foundation/week01_memory`
- `week02` -> `01_cpp_foundation/week02_oop`
- `week03` -> `01_cpp_foundation/week03_stl`
- `week04` -> `01_cpp_foundation/week04_raii`
- `week05` -> `01_cpp_foundation/week05_move_gdb`
- `test` -> `sandbox/cmake_experiments`
- `1.cpp`、`1.exe`、`song_data.dat` -> `sandbox/song_ranking/`
- `newPlan.md` -> `00_docs/roadmap.md`
- `Plan.md` -> `00_docs/roadmap_old.md`
- `prompt.md` -> `00_docs/master_prompt.md`
- `prompt_week.md` -> `00_docs/weekly_prompt.md`

已新增：

- `00_docs/trial_8_week.md`：8 周试运行说明。
- `00_docs/session_handoff.md`：本交接文档。

`.gitignore` 已加入构建目录、`.exe`、目标文件、调试文件、`.vscode` 等忽略项。

注意：执行整理前 Git 工作区已经存在历史性的删除记录和未提交变更，不能擅自回滚、重置或清理；后续操作必须保留用户现有改动。

## 4. 当前计划和决策

8 周试运行从第 6 周开始，覆盖第 6–13 周：

| 周次 | 主题 | 预期产出 |
|---|---|---|
| 第 6 周 | C++ 进阶收尾与 CMake 工程化 | 多文件 C++ 项目、CMake 构建、基础 GDB 记录 |
| 第 7 周 | Linux 命令、编译流程、文件和进程基础 | Ubuntu 实验和命令笔记 |
| 第 8 周 | 进程、信号和基础多线程 | 进程/信号/线程同步实验 |
| 第 9 周 | 网络基础与 TCP/IP | TCP/UDP 流程笔记和诊断实验 |
| 第 10 周 | Linux Socket | TCP Echo Server/Client |
| 第 11 周 | 多线程网络和消息协议 | 多客户端并发服务端，处理半包/粘包 |
| 第 12 周 | Qt Widgets、布局、信号槽 | 可运行 Qt 客户端基础界面 |
| 第 13 周 | Qt 网络、线程、JSON | 设备监控项目第一版 |

建议项目顺序：

- `tcp_server` 和 `network_client`：第 10–11 周使用。
- `device_monitor`：第 13 周综合使用，模拟设备数据并通过 Qt 界面展示。

这些名称目前是规划中的项目名，不代表用户已经完成了这些项目。

## 5. 下一步应该做什么

第 6 周计划已经生成在 `01_cpp_foundation/week06_cmake/Week06_Plan.md`。Day 1、Day 2、Day 3 已完成并报告在 Ubuntu 中构建和运行通过；当前进入 Day 4 静态库和目标依赖实践。

- CMake 多文件工程。
- 静态库/动态库的基本概念和至少一个实践。
- GDB 基础复盘。
- `constexpr`、`=default`、`=delete`、lambda、`std::function` 的必要实践。
- 一个可运行的多文件 C++ 小项目。
- Ubuntu VMware 环境下的构建命令。

Day 2 复盘曾有勾选项未更新的问题；Day 3 的 `review.md` 目前也仍有未勾选项目，后续应根据已验证的运行结果补齐，但不能把未填写的复盘误认为已完成。

每周计划不要一次生成全部 8 周的细节；只生成当前周，下一周根据实际完成情况调整。

## 6. 每日协作方式

用户重视实践。每天完成任务后会发送代码、编译输出、运行截图或问题。

检查代码时应按顺序关注：

1. 是否能编译，指出错误位置和原因。
2. 是否有逻辑错误、边界条件和资源管理问题。
3. 是否有 CMake、Linux、线程或网络工程问题。
4. 是否能解释关键设计，是否达到当日验收标准。
5. 给出下一步修改任务，不要直接替用户重写全部练习代码。

用户偏好的协作规则：

- 语法错误优先指出位置和原因，让用户自己修改。
- 练习代码应保留独立完成的价值，不直接提供完整答案，除非用户明确要求。
- 复杂任务拆成每天可以验证的小任务。
- 一次只问一个澄清问题；只有达到较高把握理解需求后才给最终路线方案。
- 发现计划过重时，应按实际进度缩小范围，而不是要求机械补课。

## 7. 绝对不要再踩的坑

- 不要把嵌入式 Linux、MCU、ROS、Qt、网络、算法同时作为当前主线；寒假实习目标需要优先形成 Qt/Linux 可展示项目。
- 不要把“看完视频”当作完成；必须有可编译代码、运行结果、README 和复盘。
- 不要继续把 `.exe`、`build`、`bulid`、`.obj`、`.pdb` 混在源码目录或提交到 Git。
- Windows 和 VMware Ubuntu 不要共用同一个构建目录；使用 `build-windows/` 和 `build-linux/`，或直接在 Ubuntu 仓库中构建。
- 不要再使用无意义的文件名，如 `1.cpp`、`test2.cpp`；使用能表达用途的英文小写名称。
- 不要把临时实验直接放进 `04_projects/`；只有能演示、能说明、能写进简历的项目才进入该目录。
- 不要删除或回滚用户已有的未提交改动；操作前先查看 `git status --short`。
- 不要把计划中提到的 `tcp_server`、`network_client`、`device_monitor` 误认为已经完成。
- 不要为了赶进度跳过代码修复、GDB 调试和项目 README；这些正是从“学过”到“能实习”的证据。

## 8. GitHub 项目策略

用户希望“自己从零实现 + 后期阅读和二次开发”结合：

- 基础阶段优先自己实现 TCP、线程、Qt 小项目。
- 项目达到可运行后，再寻找 C++、Qt、Linux、网络、工业软件、机器人上位机相关开源项目进行编译、阅读和小范围修改。
- 推荐开源项目必须说明：适合原因、编译难度、需要先掌握的知识、建议阅读目录和可做的最小修改任务。
- 不要只给项目链接；要给出具体的学习使用方式。

## 9. 交接结束时的状态格式

### 本次新增状态

- 第 6 周计划已生成：`01_cpp_foundation/week06_cmake/Week06_Plan.md`。
- 原计划中的 B 站 CMake 视频链接不可用，已改为 CMake 官方教程：`https://cmake.org/cmake/help/latest/guide/tutorial/index.html`。
- 后续不要把该失效视频作为必需资料；优先使用官方文档和实际构建练习。
- Week06 Day 1 已在 VMware Ubuntu 中完成 CMake 配置、生成、编译、链接和运行；`calculator_demo` 输出 `8`、`5`、`24`，构建通过。
- Day 1 的 `CMakeLists.txt`、`calculator.h`、`calculator.cpp`、`main.cpp` 已检查：功能和工程结构通过，仅有 include 格式和 `using namespace std` 两处风格问题待用户自行修正；下一步由用户写当天 `review.md`。
- Week06 Day 2 文件已生成：`01_cpp_foundation/week06_cmake/day02/`，包含 `example.cpp`、`exercise.cpp`、`CMakeLists.txt` 和 `review.md` 模板，主题为 `constexpr`、`=default`、`=delete`。
- Day 2 的 `exercise.cpp` 保留 TODO，不能当作已完成；需要先在 Windows 阅读并独立补全，再通过 Xftp 上传到 Ubuntu 构建。Windows 当前沙箱未完成 CMake/GCC 生成验证，Ubuntu 是正式验收环境。
- Day 2 的 `exercise.cpp` 已由用户补全，包含 `constexpr cube`、`Config` 的 `constexpr` 构造/访问函数、`=delete` 拷贝操作和默认移动操作；用户报告已在 Ubuntu 中构建运行通过。
- Day 2 `review.md` 已开始填写，但四项实践结果复选框仍为未勾选，遇到的问题部分为空；下一步需要补充 `noexcept` 的实际用途、编译期/运行期区别，并勾选已验证项目。
- Week06 Day 3 已由用户完成：`day03_example` 和 `day03_exercise` 均在 Ubuntu 中用 CMake 构建成功并运行；成绩排序、`copy_if` 筛选、`count_if` 计数、Lambda 回调、普通函数回调、按值捕获和按引用捕获均已验证。
- Day 3 运行时曾误执行 `./build-linux/`，确认目录不能直接执行，正确方式是执行 `./build-linux/day03_example` 或 `./build-linux/day03_exercise`。
- Day 3 的 `review.md` 已写入主要概念说明，但实践复选框仍未勾选；其中 `[=]`、`[&]`、`std::function` 的表述还可以继续精确化。
- Week06 Day 4 已生成：`01_cpp_foundation/week06_cmake/day04/`，包含 `CMakeLists.txt`、`include/score_lib.h`、`src/score_lib.cpp`、`src/example_main.cpp`、`src/exercise_main.cpp` 和 `review.md`。示例使用 `add_library(... STATIC ...)` 创建 `score_lib`，两个可执行目标通过 `target_link_libraries` 链接它；练习目标保留 TODO，尚未在 Ubuntu 验收。
- Day 4 曾尝试在 Windows 使用 Visual Studio 和 MinGW 做本地 CMake 检查，但分别受到 Windows SDK 权限和当前 MinGW/CMake 环境问题阻断；不能将这次环境失败当作 Day 4 源码已经通过，正式验收仍在 Ubuntu 完成。

## 9.5 本次新增：21 天冲刺计划（2026-09-17 制定）

用户反映"没有目标、每天随便学学、不学也没有负罪感"，并要求给出**每日学习目标**。已产出三份文件：

| 文件 | 作用 |
|---|---|
| `00_docs/daily_goals_21days.md` | 总纲：完成定义（8 项硬性验收 + 6 个必须能回答的面试问题）、21 天时间轴、5 个里程碑、熔断规则、每天投入方式、技术方案。**日期为硬边界，Day 编号只是进度标签；落后时砍功能，不挪日期** |
| `04_projects/device_monitor/DAILY_CARDS.md` | 每日任务卡（9月17日–10月7日，**按日期排**）。9/17–9/20 已全细化；9/21 之后为骨架，**当天再细化**，每周日重排下一周 |
| `00_docs/daily_progress.md` | 打卡表、欠账台账、熔断计数、砍功能清单、里程碑进度 |

**配套 skill（已安装）**：`daily-goal-sprint`，位置 `C:\Users\chengang\.dsh\skills\daily-goal-sprint\SKILL.md`（用户级，任何工作目录的会话均可加载）。
新会话开始时**应加载该 skill**，由它负责日级节奏、验收判定、欠账与范围调整；技术讲解仍由 `learn-by-building` 负责，两者分工不重叠。该 skill 已写明：以打卡表实际记录为准、只细化当天卡片、不提前写死后续天、四层审查（编译→逻辑→工程→可解释性）、证据三件套缺一不算完成、砍功能优先于拖日期。

**冲刺目标**：10/7 前把 `device_monitor` 做成"通用串口/网络调试 + 实时曲线"工具，复刻 SerialTest 核心（不复制其源码），**亮点定为"多线程收发 + UI 不卡顿"**。

**关键时间约束**：9/25–10/7 中秋国庆连休 13 天，10/8 开学，所以 **10/7 是硬边界**。在校期间（9/17–9/24）用户每天可稳定投入 3–4 小时，周六家假日 2 小时；假期每天 6–7 小时；总量约 106 小时，无富余。

**排期规则（用户明确要求"按日期排"）**：任务卡与打卡表**一律以日历日期为主键**（9月17日 → 10月7日），"Day N" 只是便于与 git 提交对照的进度编号。**日期永不后移**：某天没做完，欠的任务仍留在那个日期下面，不倒填到后面的日期；落后时只能砍功能或当周加时长，结束日期恒为 10月7日。

**本次核实到的两个重要事实（后续会话不要搞错）**：

1. **`C:\Qt\6.11.0\mingw_64` 未安装 Qt SerialPort 模块**（该目录下无 `Qt6SerialPort`，`lib\cmake` 中只有 `Qt6Network`/`Qt6Sql`/`Qt6Charts` 等）。因此**串口不能放在关键路径上**；方案是先用 `QTcpSocket` + 虚拟串口完成收发/线程/分包/曲线，再按需用 `C:\Qt\MaintenanceTool.exe` 补装 Serial Port 模块。用户**目前没有任何串口硬件**，硬件被视为加分项并明确推迟。
2. **`MessageParser` 存在待收口问题**：`handleline`（`include/MessageParser.h` 第 20 行）拼写不规范；解析失败时静默 `return`（`src/MessageParser.cpp` 第 26、27 行），应改为发出失败信号以便记日志；`mainwindow.h` 的 `pending_` 成员未被使用，需清理或接线。

**协作方式变更**：每天由用户提出"今天 Day N"后，会话应**只细化当天那张卡**（给具体步骤与文件清单），并在用户提交代码后按"编译 → 逻辑 → 工程 → 可解释性"审查。**不要提前把 9/21 之后的任务细节写死**，每周日（9/20、9/27、10/3–10/4）按实际进度重排下一周。

每次较长工作结束时，建议更新本文件或在对应文档记录：

```text
已完成：
当前问题：
尚未完成：
下一步：
新增踩坑：
相关文件：
```

## 10. 最新项目进度（2026-09-07）

`04_projects/device_monitor` 阶段 1 已完成（QTimer 定时刷新 + 超限告警与日志记录）：

- `MainWindow` 新增 `QTimer* refresh_timer_` 成员，采用 `new QTimer(this)` 交由 Qt 对象树管理生命周期；`setupUi()` 中连接 `QTimer::timeout` → `MainWindow::refreshDeviceData`，并 `start(1000)`。
- `refreshDeviceData()`：取 `device_list_->currentRow()`，防 `-1`/越界后，用 `QRandomGenerator::global()->bounded(...)` 让选中设备温度（20~60）、电压（1~5）每 1 秒在区间内随机变化；再判断是否超限（温度 > `kMaxTemperature`=45 或电压 > `kMaxVoltage`=4），用 `Device::alarm_` 只在「正常→超限」「超限→正常」转折点各写一次日志，最后调 `updateDeviceDetails(row)` 刷新右侧详情。
- `include/device.h` 已加 `bool alarm_ = false;`；`create data`：`constexpr double kMaxTemperature/kMaxVoltage` 放在 `mainwindow.cpp` 文件作用域。
- 注意坑：`QRandomGenerator::bounded(lo, hi)` 右边界是开区间，电压取 1~5 需写 `bounded(1, 6)`；`QString::arg` 占位符编号要与 `.arg` 数量对应（`%1 %2 %3`）；`#include <QRandomGenerator>` 后不要残留多余字母；`log_view_` 是 `QTextEdit`、写一行用 `append(QString)`；clang 的 `range-loop-detach` 与 `DeadStores`（`row` 未读）是警告/误报，不影响运行。
- `cmake --build build-windows-qt -j2` 构建通过（Qt Creator 在 `build/Desktop_Qt_6_11_0_MinGW_64_bit-Debug` 构建亦可），运行现象已验证。
- 已更新 `04_projects/device_monitor/docs/architecture.md` 与 `README.md` 记录。

下一次进入项目：不要重复讲解上述基础。先检查实际文件和 Git 状态。阶段 1 已收官；阶段 2（配置与历史记录）已完成（保存配置 / 读取配置 / 日志写文件）；阶段 3（Model/View 重构）已完成（`DeviceListModel : QAbstractListModel` + `QListView` + `setModel` + `QItemSelectionModel::currentChanged`，自定义角色 `TemperatureRole/VoltageRole/OnlineRole`，`DisplayRole` 显示"名字 温度xx"，`deviceDataChanged(row)` 发 `dataChanged`）；阶段 4 第一版已完成——新增 `TempChartWidget : QWidget`（`QPainter` 在 `paintEvent` 自绘温度历史折线，`QVector<double> points_` + `kMaxPoints=60` 滚动限长，坐标映射 + `drawPolyline`），窗口在「分栏」与「日志」之间新增"温度曲线"面板，`refreshDeviceData` 中调用 `temp_chart_->addValue(device.temperature_)`。下一步可选：按设备分别记录历史（现只记当前选中设备）、电压曲线、X 轴时间刻度，或进入阶段 5（通信接入），或先 `git` 提交当前成果（整个仓库从第 2 周起未提交）。尚未做串口、网络、线程、数据库或复杂图表。

补充：阶段 4 增强已完成——`TempChartWidget::paintEvent` 加了 Y 轴刻度（20~60，每 10 一格）、浅灰水平网格线、两条轴；用 `QRectF plot`（控件尺寸减边距）作绘图区，曲线与网格共用同一坐标映射（基于 `plot` + 固定范围），因此对齐。相关新函数已补进 `docs/qt_cheatsheet.md` 的"绘图"一节（`drawLine`/`drawText`/`QPen`/`QColor`/`QRectF`/`QString::number`/`Qt::AlignRight|AlignVCenter`/坐标映射公式）。

补充 2：**阶段 5 已开始，通信协议草案已完成** → `04_projects/device_monitor/docs/protocol.md`（文本 JSON、一条消息一行、以 `\n` 分隔、序列化必须用 `QJsonDocument::Compact`；字段 `id`/`temperature`/`voltage` 必填、`online`/`ts` 可选；接收用"缓冲区 + 循环切 `\n`"解决粘包/半包；解析失败丢一条并继续）。**用户当前没有串口硬件**，因此下一步实施路线是：先做"模拟数据源 + 协议解析器"把边界/粘包半包/容错跑通（不依赖硬件与 Qt SerialPort 模块），再用 `QSerialPort` 实现同一接口接真设备。协议前置概念（串口/TCP/端口/异步 `readyRead`/缓冲区/粘包半包/容错）已讲给用户。

## 11. 补充说明（2026-09-07，2026-09-11 更新）

- 用户新增「三层教学法」需求，已嵌入 `learn-by-building` skill 的 `references/three-layer-feature.md`，并在 `teaching-protocol.md` 中挂接：凡教「用户要实现的新功能」，按 数据对象→业务逻辑→文件/声明 三层拆解、每层确认。后续会话若下载新 skill 或重载可生效。
- 本次还讲清了 `const` vs `constexpr`、`QRandomGenerator::global()->bounded`、`QTimer` 信号槽发送者不是用户控件、`QListWidget::currentRow()` 从 0 起且未选中为 -1。
- **用户的学习痛点与对策**：用户反映"业务逻辑能讲清，但记不住 Qt 的类/函数名"。对策已落地：
  - 新建 `04_projects/device_monitor/docs/qt_cheatsheet.md`（"任务 → 类 → 关键函数"索引 + 命名规律 + 本项目套路 + 踩坑清单）。
  - README「学习规则」第 7 条、`docs/architecture.md`「个人 Qt 速查表」一节均已写明：**每学会一个新类/函数就往速查表加一行**，记不住时先查表再用 Qt Creator `Ctrl+Space`/`F1`，**不要背 API**。
  - 新会话遇到用户"不知道用什么控件/函数"时，引导其**先查 `docs/qt_cheatsheet.md`**，并教"任务→类"的对应与查找方式，而不是直接给答案。
