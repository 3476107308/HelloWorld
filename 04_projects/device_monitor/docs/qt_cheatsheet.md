# device_monitor Qt 速查（个人索引）

> 用途：**不是用来背 API**，而是"**任务 → 类 → 关键函数**"的索引。
> 写代码前先查这里；具体函数细节再用 Qt Creator 的 `Ctrl+Space` / `F1` 确认。
> **每学会一个新的类/函数，就往下面加一行。** 保持简短、只记"有这个东西"。

---

## 一、记不住时的三步查找法

1. **补全**：打出对象名 + `->`（或 `.`），按 `Ctrl+Space` → 列出该对象**所有可用函数**。
2. **找类**：打 `new Q` + `Ctrl+Space` → 列出所有 `Q` 开头的类。
3. **看文档**：光标停在类/函数上按 `F1` → 打开官方文档；按**任务**搜（"read file"）而不是按类名搜。

---

## 二、任务 → 类 → 关键函数（本项目已用到）

### 显示与文本
| 我想干什么 | 类 | 关键函数 / 信号 |
|---|---|---|
| 显示一段文字 | `QLabel` | `setText()` |
| 多行文本 / 日志显示 | `QTextEdit` | `append()`、`setPlainText()`、`setReadOnly()` |

### 列表与模型
| 我想干什么 | 类 | 关键函数 / 信号 |
|---|---|---|
| 简单列表（自带数据） | `QListWidget` | `addItem()`、`currentRow()`、`currentRowChanged` |
| 模型驱动列表 | `QListView` | `setModel()`、`selectionModel()` |
| 列表模型基类 | `QAbstractListModel` | `rowCount()`、`data(index,role)`、`beginResetModel/endResetModel`、`dataChanged` |
| 管理选中 | `QItemSelectionModel` | `currentChanged`、`setCurrentIndex()` |
| 定位/角色 | `QModelIndex` / `Qt::ItemDataRole` | `index.row()`、`Qt::DisplayRole`、`Qt::UserRole` |

### 按钮与输入
| 我想干什么 | 类 | 关键函数 / 信号 |
|---|---|---|
| 按钮 | `QPushButton` | `clicked` 信号 |
| 单行输入 | `QLineEdit` | `text()` |
| 勾选框 | `QCheckBox` | `isChecked()` |
| 下拉选择 | `QComboBox` | `addItem()`、`currentText()` |

### 布局与容器
| 我想干什么 | 类 | 关键函数 |
|---|---|---|
| 竖 / 横 / 表单 / 网格 布局 | `QVBoxLayout` / `QHBoxLayout` / `QFormLayout` / `QGridLayout` | `addWidget()`、`addRow()` |
| 带标题分组框 | `QGroupBox` | 构造传标题 |
| 可拖动分栏 | `QSplitter` | `addWidget()`、`setStretchFactor()` |
| 主窗口 | `QMainWindow` | `setCentralWidget()` |
| 基础控件 / 中心区 | `QWidget` | `update()`、`rect()`、`width()`、`height()` |

### 定时
| 我想干什么 | 类 | 关键函数 / 信号 |
|---|---|---|
| 周期性触发 | `QTimer` | `start(ms)`、`stop()`、`timeout` 信号 |

### 文件 I/O
| 我想干什么 | 类 | 关键函数 |
|---|---|---|
| 读写文件 | `QFile` | `open(ReadOnly/WriteOnly/Append\|Text)`、`readAll()`、`write()`、`close()`、`errorString()` |
| 字节串 | `QByteArray` | `toUtf8()`、`+ "\n"` |
| 时间戳 | `QDateTime` | `currentDateTime().toString("yyyy-MM-dd hh:mm:ss")` |

### JSON
| 我想干什么 | 类 | 关键函数 |
|---|---|---|
| 整体文档（序列化/解析） | `QJsonDocument` | `toJson(Indented)`、`fromJson(bytes,&err)`、`array()`、`object()`、`isNull()` |
| 对象 / 数组 | `QJsonObject` / `QJsonArray` | `obj["k"]=v`、`obj["k"].toString()/toBool()/toDouble()`、`append()` |
| 值 / 解析错误 | `QJsonValue` / `QJsonParseError` | `toObject()`、`error`、`errorString()` |

### 绘图
| 我想干什么 | 类 | 关键函数 |
|---|---|---|
| 自绘（在控件上画） | `QPainter` | `QPainter(this)`、`fillRect()`、`setPen()`、`drawPolyline()` |
| 画直线（网格/轴） | `QPainter` | `drawLine(QPointF, QPointF)` |
| 画文字（刻度数字） | `QPainter` | `drawText(QRectF, Qt::Alignment, QString)` |
| 设笔（颜色/粗细） | `QPen` | `QPen(QColor(220,220,220), 1)`、`QPen(Qt::blue, 2)` |
| 颜色 | `QColor` | `QColor(r,g,b)`、`Qt::black`、`Qt::blue` |
| 矩形（绘图区/文字框） | `QRectF` | `QRectF(x,y,宽,高)`、`left()/top()/right()/bottom()/width()/height()` |
| 数字转文字 | `QString` | `QString::number(int(v))` |
| 文字对齐 | `Qt::AlignmentFlag` | `Qt::AlignRight \| Qt::AlignVCenter` |
| 请求重绘 / 重写绘图 | `QWidget` | `update()`、`paintEvent(QPaintEvent*)` |
| 点 / 折线点集 | `QPointF` / `QPolygonF` | `QPointF(x,y)`（**浮点**）、`poly << point` |
| 坐标映射（数据→像素） | — | `x = plot.left() + i*plot.width()/(kMaxPoints-1)`；`y = plot.top()+plot.height() - (值-min)/(max-min)*plot.height()` |

### 随机与常量
| 我想干什么 | 类/关键字 | 用法 |
|---|---|---|
| 随机数 | `QRandomGenerator` | `global()->bounded(lo,hi)`（**右边界开区间**） |
| 编译期常量 | `constexpr` | `constexpr double kX = 1.0;` |

### 调试
| 我想干什么 | 用法 |
|---|---|
| 输出调试信息 | `qDebug() << ...;` |

### 弹窗（项目里尚未用，常见）
| 我想干什么 | 类 | 关键函数 |
|---|---|---|
| 提示 / 警告框 | `QMessageBox` | `information()`、`warning()` |
| 选择文件 | `QFileDialog` | `getOpenFileName()` |

### 信号槽
| 我想干什么 | 用法 |
|---|---|
| 连接信号与槽 | `connect(sender, &Sender::signal, receiver, &Receiver::slot)` 或接 lambda |
| 发射信号 | `emit signalName(args);`（`dataChanged` 等**内置信号不用自己定义**） |

---

## 三、Qt 命名规律（用来"猜"名字）

- 类以 **`Q`** 开头：`QTimer`、`QLabel`。
- **`...Widget`** = 便捷控件（**自带数据/模型**）；**去掉 `Widget`** = 纯视图（**要 `setModel`**）。例：`QListWidget` vs `QListView`、`QTableWidget` vs `QTableView`。
- 属性：**`setXxx()`** 设置，**`xxx()`** 读取（`setText()`/`text()`、`setModel()`/`model()`）。
- 信号多为**过去式**：`clicked`、`textChanged`、`currentRowChanged`、`timeout`。
- 常用动词：`add*`、`insert*`、`remove*`、`clear`、`set*`、`current*`。

---

## 四、本项目的常用套路

- **搭界面**：`new 控件(父对象)` → `layout->addWidget/addRow` → `setCentralWidget`。
- **模型驱动列表**：`QListView` + 自定义 `QAbstractListModel` + `setModel` + `selectionModel()->currentChanged`。
- **数据变了通知视图**：`emit dataChanged(index,index)`（细粒度）／ `beginResetModel()+endResetModel()`（整体）。
- **事件回调**：重写 `protected` 虚函数（`paintEvent` 等），**Qt 自动调用，你不自己调**。
- **定时刷新**：`QTimer::timeout` → 槽函数。
- **读写配置**：`QJsonDocument::toJson/fromJson` + `QFile`。

---

## 五、踩过的坑（备忘）

- `QRandomGenerator::bounded(lo,hi)` **右边界开区间**：要 1~5 写 `bounded(1,6)`。
- `QString::arg` 占位符编号要与 `.arg` 数量对应（`%1 %2 %3`，别写 `%4`）。
- **`QListView` 没有** `currentRow()`/`clear()`/`addItem()`/`setCurrentRow()`（那是 `QListWidget` 的）→ 用 `selectionModel()`／模型 `reset`／`setCurrentIndex`。
- `data()` 里**漏 `Qt::DisplayRole`** → 列表空白（视图用 DisplayRole 取显示文字）。
- **对象/继承需要 `#include` 完整定义**；只有**指针/引用**才能用前置声明。
- 控件必须**"先 `new` 再用"**，否则空指针崩溃。
- `QFile::write` **不自动换行**，要自己加 `"\n"`；字符串写文件要 `.toUtf8()`。
- 日志文件用 **`QIODevice::Append`** 追加（`WriteOnly` 会覆盖）。
- **`QTimer` 的发送者是定时器自己**（`timeout` 由事件循环触发，无需点击）。
- 同一个控件**别加进布局两次**。
