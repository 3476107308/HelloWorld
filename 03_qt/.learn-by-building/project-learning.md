# 项目学习记录：Qt 自定义信号槽登录小项目（CustomSignalAndSlot）

> 本文件是 learn-by-building 的项目学习记录。用途：让下一次会话快速恢复进度，只记**要点 + 可观察证据**，不存代码全文、不含隐私。下次会话先读本文件再继续。

## 基本信息

- 记录日期：2026-09-01
- 环境：Qt 6.11.0 (Desktop Qt 6.11.0 MinGW 64-bit) + CMake 4.3.0 + g++ 15.2.0，Qt Creator 编译运行
- 项目：`03_qt/CustomSignalAndSlot` → 已改为「登录界面」演示，可运行、可弹窗
- 关联项目：`03_qt/HelloQt`（基础窗口+按钮）、`03_qt/SignalAndSlot`（内置信号槽，窗口最大化/最小化/关闭）

## 已完成并验证（有运行证据）

以下概念均在**真实运行**中观察通过，视为「已掌握」：

- **Qt 工程结构**：`QApplication` + `QWidget`、`main()` + `QCoreApplication::exec()` 事件循环；`CMakeLists.txt` 用 `find_package(Qt6 ...)` + `qt_add_executable` + `target_link_libraries`。
- **.ui 设计器**：拖控件、设置 `objectName`、属性编辑器的三层层级（QObject → QWidget → 具体控件类），垂直布局 `verticalLayout`。
- **内置信号槽**：`connect(ui->btn..., &QPushButton::clicked, 窗口, &QWidget::showMaximized)` 等（SignalAndSlot 验证）。
- **自定义带参信号**：`signals: void loginSuccess(const QString &username);` —— 信号只声明、没有函数体，实现由 moc 生成。
- **新式 connect**：`connect(发送对象, &Sender::signal, 接收对象, &Receiver::slot)`，编译期类型检查。
- **emit 触发信号**：`emit loginSuccess(name)` 把数据（用户名）传给跨对象接收方（main 里的 lambda）。
- **跨对象解耦**：`MyWidget` 发信号，`main` 中 lambda 接收。观察到弹窗「欢迎, 1111」。
- **守卫子句 / 提前返回**：`if (name.isEmpty()) { warning(...); return; }`，非法输入先拦截。观察到弹窗「请输入用户名」。
- **QMessageBox**：`information()`（蓝 i，nullptr 父）、`warning()`（黄三角，this 父）；模态阻塞、返回 `StandardButton`。

## 待巩固 / 未完成

- `01_cpp_foundation/week06_cmake/day04` 的 `exercise_main.cpp` 仍含 TODO，**尚未在 Ubuntu 验收**（不要当作已完成）。
- 前几周 C++ 基础代码仍存在零散低级错误与工程规范问题（见 `00_docs/session_handoff.md`），应通过修复/测试/解释巩固，不重学全量。
- Git 提交不规律：近期 Qt 代码未提交；仓库当前存在大量未提交改动，操作前先看 `git status --short`。

## 下一步建议（按 roadmap，节奏主动降速）

- 学期刚开学（2026-08-31 开课），家教+健身+英语并行，**每次会话只推进一小步**，避免一次塞太多。
- 方向：Qt 布局/容器、`QString/QList/QMap`，`QFile` 文件读写（text editor 练习），再到 Qt 网络/多线程（`device_monitor`）。
- 保持"每次都有可编译、可运行产物 + 截图 + 复盘"的验收标准。

## 本次坑 / 提示

- `&` 的两种含义要分清：对象地址 `&w` vs 成员函数指针 `&类::函数`。
- QMessageBox 第一个参数 `this`（跟随窗口）vs `nullptr`（独立）。
- 一个值用多次 → 先存局部变量；只用一次 → 可直接链式。
- 学习节奏上：允许断、不许弃（手机交接文档里的自我要求）。

## 复盘暴露的高频错点（2026-09-01 主动回忆）

学习者在独立重做 ①②③④ 时暴露的记忆断点，按"命根子规律"归类，下次重点针对复习：

1. **设计器控件必须 `ui->名字`**：`username`、`pwdEdit`、`btnLogin` 都要 `ui->` 访问，不能裸写（写成了 `username->text()`、`btnLogin`）。
2. **类名大小写 + 拼写**：`QPushButton`（大写 B）、`QMessageBox`（大写 M）；`warning`（不是 waring）、`clicked`（不是 click）、`loginSuccess`（不是 oginSuccess）、`QMessageBox`（不是 QMessage）。
3. **信号名三处一致**：`loginSuccess` 在①声明、③`emit`、④`connect` 三处一模一样（曾拼成 `btnclick` / 差点拼成别的）。名字可自选，但定了就必须全程一致。
4. **`connect` 的信号必须属于"发送者类"**：④里发送者是 `&w`(MyWidget)，所以信号是 `&MyWidget::loginSuccess`；不能套用②里按钮的 `&QPushButton::clicked`（那是 `ui->btnLogin` 这个发送者的信号）。判断依据：`&` 后的类名 = 发送者对象的类。
5. **槽名固定为 `onBtnLoginClicked`**（忘了写）。

下次练习建议：新建空项目、不看答案，15 分钟内独立重做完整流程 —— `.ui` 拖控件+布局 → 头文件声明信号/槽 → 构造里 `connect` → 槽(判空 + `QMessageBox::warning` + `return` + `emit`) → main 跨对象接收弹窗。做完能讲清"点→判→发→收→弹"即达标。
