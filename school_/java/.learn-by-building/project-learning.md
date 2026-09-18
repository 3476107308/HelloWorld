# 项目学习记录 — Java 课程环境（JDK 25）

> **这是什么**：学习进度记录，让下次你问"为什么"时我不用从头讲一遍，也能接着上次往下走。
> 只记进度，不记聊天原文、密码或无关个人信息。**交作业时不要打包这个隐藏文件夹**。随时可以说"删掉它"。

## 项目目标
给学校 Java 课搭好本机环境：装 JDK 25 → 命令行编译运行 Java → 让 IDEA 用上这个 JDK → 理解 JDK / PATH / JAVA_HOME / IDE 各自负责什么。

## 课程信息（读老师发的资料得到）
- 南通大学 人工智能与计算机学院，《Java程序设计基础》，任课教师：陆培军
- 实验1「Java开发环境搭建及编程基础」任务1 = 安装配置流程 + **截图**（6 个截图点见 JDK 文档第 8 节）
- 老师给 IDE：IDEA Community 2024.3.4.1 **解压版**，预期解压到 `D:\ideaIC-2024.3.4.1.win`，跑 `bin\idea64.exe`
- 资料：`school_\java\(老版)JavaEE基础教程实验指导与习题解析.pdf`、`Idea下载安装.docx`、`Java程序设计基础百度网盘链接.txt`

## 环境与进度

### ✅ JDK 25（2026-09-15 装完并实测通过）
| 项目 | 实际值 |
|---|---|
| 方式 | `winget install --id Microsoft.OpenJDK.25 -e`（winget 1.29.290） |
| 版本 | Microsoft Build of OpenJDK **25.0.4.1+1 (x64)** |
| 位置 | `C:\Program Files\Microsoft\jdk-25.0.4.101-hotspot\`（373 MB + MSI 缓存 189 MB） |
| PATH / JAVA_HOME | 系统级均已自动配好 ✅（推翻我先前的"winget 不设 JAVA_HOME"预警，issue #122 已被新版修掉） |
| 实测 | `javac 25.0.4.1`；`java -version` → `25.0.4.1 2026-08-18 LTS`；字节码 `major version: 69` |

**教训**：issue 是历史信息，结论必须用本机实测确认。

### ✅ VS Code 路线（2026-09-15 跑通，用户自测）
- 装好 `Extension Pack for Java`：7 个扩展到位（redhat.java 1.56 + debug + test + maven + gradle + dependency），JDT LS 已下载（51 MB / 114 jar）
- 在 `projects\Lab01`（我建的脚手架）跑通：输出 `Hello, Java 25!` / `java.version = 25.0.4.1`，状态栏 `Java Ready`
- 用户说明：**VS Code 项目只是测试，已删除**；未来以 IDEA 为主

### ⚠️ IDEA 路线（2026-09-15 已成功启动、已建项目，但有两个待处理项）
- IDEA 2024.3.4.1 已解压到 **`D:\ideaIC-2024.3.4.1.win\`**（实测确认存在，`bin\idea64.exe` 可用）
- 首启对话框走完；已用 `Add sample code` 建过项目并看到 `Main.java` 示例代码
- 用户已删除测试项目；**尚未建正式项目**
- **兼容性问题已实测坐实（铁证）**：`.idea\misc.xml` 中 `languageLevel="JDK_X" project-jdk-name="ms-25"` —— **`JDK_X` 是 IDEA"不认识该 JDK 版本"的占位符**；后果：① 右下角「实验性功能警报」② 编译/运行被自动加 `--enable-preview`。修法：`Ctrl+Alt+Shift+S → 项目 → Language level = 21`（**别选「SDK 默认」**，会回到 JDK_X）
- ✅ 正式项目已建成：`D:\AAA study\school_\java\projects\Lab01`（**单层，无嵌套**），示例代码运行成功（`进程已结束，退出代码为 0`），用的是 JDK 25 的 `java.exe`
- ⚠️ 项目树顶部仍有一个 `week01` 项目标签，但 `recentProjects.xml` 里只记录了旧的嵌套项目路径 → week01 的来源待确认（疑似课程资料里的项目或已删除的残留）
- 用户已修正认知：**"爆红"≠ 报错**——红点=断点、蓝斜体=引导提示、整行淡红底=引导高亮；真错误是红色波浪线 + Run 失败
- **踩到的坑**：新建项目时 `Location` 填的是**父目录**、IDEA 在其下用 `Name` 建子文件夹 → 用户得到 `projects\Lab01\Lab01`（嵌套在 VS Code 项目里）。已说明正确填法：`Location = ...\school_\java\idea-projects`、`Name = Lab01`
- **待办**：建桌面快捷方式（对 `bin\idea64.exe`，**不是** `jetbrains_client64.exe`）

## 机器上可用的 JDK 清单
| 路径 | 版本 | 用途 |
|---|---|---|
| `C:\Program Files\Microsoft\jdk-25.0.4.101-hotspot` | 25.0.4.1 | ✅ 主角：编译/运行用户代码 |
| `C:\Program Files\Android\openjdk\jdk-21.0.8` | 21.0.8 | Android Studio 自带；IDEA 兼容性兜底 |
| `C:\Program Files (x86)\Android\openjdk\jdk-17.0.14` | 17.0.14 | 老版残留，勿用 |
| VS Code 扩展自带 JRE | 21.0.12.1 | 只跑语言服务器（`...\redhat.java-1.56.0-win32-x64\jre\`） |

## 概念进度
`introduced` 讲过 / `reinforced` 复现过 / `mastered` 有可观察证据 / `needs-review` 需换讲法。
**Agent 验证 ≠ 学习者掌握。**

| 概念 | 阶段 | 证据 |
|---|---|---|
| JDK vs JRE vs JVM | reinforced | 学习者主动问"是不是装了编译器和解释器"→ 已纠正：JDK 含 javac + java/JVM + 类库；JVM 是解释+JIT 混合 |
| PATH 环境变量 | reinforced | 实测 PATH 中只有 JDK 25 的 bin，`where.exe java` 唯一命中 |
| JAVA_HOME | reinforced | 系统变量已确认存在 |
| 环境变量进程启动时读取 → 改完要重开终端/重启 IDE | reinforced | 实测 VS Code 启动时间（08:27）晚于 JDK 安装（08:04）→ 结论：其环境本就正确，先前"必须 reload"的建议在本机不成立 |
| `where.exe`（PATH 查证工具） | reinforced | 用本机真实输出演示：java/javac 唯一命中、curl 三命中体现优先级、找不到时退出码 1 |
| `java -version` 输出走 stderr | introduced | 实测复现 |
| `javac` → `java`（`java` 不带 `.class` 后缀） | reinforced | 用户 VS Code 运行命令中可见 `-cp ...\bin 'Hello'`；**待其在 IDEA 中再跑一遍** |
| 字节码版本绑定 JDK（major 69 = Java 25） | introduced | `javap` 实测 69 |
| 安装版（MSI 登记系统）vs 解压版（绿色免登记） | reinforced | 用 JDK(MSI) vs IDEA(解压版) 对照；并解释解压版为何不自动建快捷方式、不注册文件关联 |
| IDE 与 JDK 的关系：IDE 不编译，它调用 JDK 的 javac | reinforced | 用其 C++ 工作流（VS Code + MSYS2 g++）作同构类比 |
| "工具用的 Java" ≠ "你代码用的 Java" | introduced | 实测：redhat.java 扩展自带 JRE 21.0.12.1 跑语言服务器；JDK 25 才编译/运行用户代码 |
| classpath（`-cp` / `-classpath`） | introduced | 从用户 VS Code 的实际运行命令中解剖：`-cp ...\Lab01\bin`；IDEA 对应 `out\production\...` |
| Project SDK / Language level | introduced | 实测遇到 preview 语言级别警告 → 待用户自行改为 21 |
| VS Code 扩展架构（Java 支持靠扩展包） | reinforced | 扩展包与 JDT LS 均已就位并跑通 |

## 待办
1. [ ] **建正式项目**：`Location = D:\AAA study\school_\java\idea-projects`、`Name = Lab01`、`Build system = IntelliJ`、JDK = 25
2. [ ] 把 **Language level 改成 21**，消除「实验性功能警报」；并确认 `文件 → 新项目设置 → 结构…` 是否存在（存在则可"设一次、以后新项目都生效"；不存在则每个项目改一次=方案①）
3. [ ] 建桌面快捷方式（对 `bin\idea64.exe`）+ 固定到任务栏
4. [ ] 在 IDEA 里跑通第一个类（`Shift+F10`），对比 Run 窗口第一行的 java.exe 路径是否指向 JDK 25
5. [ ] 收齐实验1 任务1 截图；顺手在终端跑 4 条验证命令并截图
6. [ ] 用户在 `school_\java\code\` 自己跑一遍 `javac HelloWorld.java` / `java HelloWorld`

## 已交付
- `school_\java\JDK25安装配置说明.md`（实际结果表、报错对照表、截图清单、空间与搬盘、§11）
- `school_\java\IDEA2024.3.4.1安装与JDK配置说明.md`（§1 下载点选顺序、§3 快捷方式与 bin 清单、§7 兼容性决策树、§12 VS Code↔IDEA 速查、§13 首次进入 IDEA 见到的每样东西）
- `school_\java\code\HelloWorld.java`（环境自检用）
