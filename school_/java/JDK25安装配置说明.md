# JDK 25 安装配置说明（Windows 11 x64）

> **这份说明是干嘛的**：给你自己的电脑装 Java 开发环境，直接对应《Java程序设计基础》**实验1 任务1**（"Java开发环境的安装配置流程，请给出配置流程与截图"）。
> 每一步都写成三段：**做什么 → 为什么 → 该截哪张图**。
>
> ✅ **2026-09-15 状态：本机已按本文装完并实测通过**（实际结果见第 0 节，截图清单见第 8 节）。

---

## 0. 你电脑的现状 + 实际安装结果（2026-09-15）

### 装之前

| 检查项 | 结果 | 说明 |
|---|---|---|
| 操作系统 | Windows 11 24H2（build 26100），**x64** | 要下 **x64** 的 Windows 安装包 |
| `java` / `javac` | 都不存在 | 电脑上**一个 Java 都没有**，不是"装了没配好" |
| `JAVA_HOME` | 空 | 还没指向任何 JDK |
| winget | 存在，版本 **1.29.290** | 可用 |

### ✅ 实际结果（装完，已实测通过）

| 项目 | 实际值 |
|---|---|
| 安装方式 | `winget install --id Microsoft.OpenJDK.25 -e` |
| 装到的版本 | **Microsoft Build of OpenJDK 25.0.4.1+1 (x64)** |
| 安装位置 | `C:\Program Files\Microsoft\jdk-25.0.4.101-hotspot\` |
| 系统 PATH | 已自动加入 `C:\Program Files\Microsoft\jdk-25.0.4.101-hotspot\bin` ✅ |
| 系统 JAVA_HOME | 已自动设置为 `C:\Program Files\Microsoft\jdk-25.0.4.101-hotspot\` ✅ |
| 实测 | `javac 25.0.4.1`；`java -version` → `openjdk version "25.0.4.1" 2026-08-18 LTS`；`HelloWorld` 编译运行成功，字节码 `major version: 69` |
| 备注 | 网上流传的"winget 不设 JAVA_HOME"（[issue #122](https://github.com/microsoft/openjdk/issues/122)）**在本机不成立**——winget 1.29 已自动配好 PATH 与 JAVA_HOME，且没有弹 UAC |

> 上面这张表可以直接抄进实验报告的"2. 实验环境"。

---

## 1. 先分清三个名字（30 秒版）

| 名字 | 一句话 | 你需要吗 |
|---|---|---|
| **JVM** | Java 虚拟机，真正"运行"你程序的引擎 | 随 JDK 自带 |
| **JRE** | JVM + 运行需要的类库，**只能跑**现成的程序 | ❌ 写作业不够用 |
| **JDK** | JRE + **`javac` 编译器**等开发工具，**能编译**源码 | ✅ 就要装它 |

> 一句话：**JDK 是"能做饭的厨房"，JRE 只是"能热饭的微波炉"**。你要写作业，必须装 JDK。
> JDK 25 是 **LTS**（长期支持版），学校课程用它没问题。

---

## 2. 下载（二选一）

### 路线 A：winget 一条命令 ✅ **本次实际用的就是这条**

```powershell
winget --version                              # 先确认 winget 活着
winget install --id Microsoft.OpenJDK.25 -e   # 再安装
```

- `install` = 安装；`--id Microsoft.OpenJDK.25` = 指定装"微软构建的 OpenJDK 25"；`-e` = 精确匹配（防止装到名字相似的其他包）。
- **本次实测没有弹 UAC**，winget 自己在后台完成了提权；如果你以后重装时弹了 UAC，点"是"即可。
- ⚠️ 网上说 winget 静默安装**不设 `JAVA_HOME`**（[microsoft/openjdk#122](https://github.com/microsoft/openjdk/issues/122)）——**本机实测已被修好**，PATH 和 JAVA_HOME 都自动配上了。**但仍然要照第 4 节验证一遍**，别靠猜。

### 路线 B：浏览器下载官方 MSI（winget 不能用 / 想自己选安装路径时用）

**直链**（微软官方短链，免注册账号）：

```
https://aka.ms/download-jdk/microsoft-jdk-25-windows-x64.msi
```

直链失效就从这两个入口进：

- 官方下载页：<https://learn.microsoft.com/zh-cn/java/openjdk/download> → 选 **Windows x64** 的 `.msi`
- OpenJDK 首页：<https://www.microsoft.com/openjdk>

**下载慢的备选**：清华镜像站有 Adoptium(Temurin) 的 25 版（`mirrors.tuna.tsinghua.edu.cn` → `Adoptium` → `25` → `jdk` → `x64` → `windows`）。Temurin 和微软版是**同一份 OpenJDK 源码**构建的，功能一样。

> 📸 **截图点 ①**：本次走的是路线 A → 截 **`winget install` 命令 + 它的输出**。
> （如果你以后改用路线 B，就截下载页面 / 安装包文件名。）

---

## 3. 安装（路线 B 的向导细节，留作以后重装参考）

1. **双击 `.msi`**（没弹界面就等几秒，MSI 启动比 exe 慢）。
2. 一路 `Next`，到 **「Custom Setup / 自定义安装」** 页**停一下**，展开树看：
   - `Add to PATH` ← **必须勾**（否则终端敲 `java` 会说"不是内部或外部命令"）
   - `Set JAVA_HOME variable` ← **建议勾**（以后 IDEA / Maven 要用）
   - `JavaSoft (Oracle) registry keys` ← 默认即可
   - 判断标准：图标是**硬盘**样式 = 会装；带**红叉** = 不装。不确定就右键选 `Will be installed on local hard drive`。
3. **安装路径怎么选**：
   - 默认 `C:\Program Files\Microsoft\jdk-25.x.x-hotspot\`，**最省事**，配置工具都认得（本次就是这里）。
   - 想装 D 盘请守三条：**纯英文、不带空格、层级别太深**，例如 `D:\Dev\jdk25`。
     ⚠️ 不要放 `D:\软件应用\`（中文路径）或 `D:\My Java\`（含空格）——部分老工具解析这种路径会出莫名其妙的错。
   - 自己指定路径时建议**不带版本号**（`D:\Dev\jdk25`），以后升级覆盖同一文件夹，`JAVA_HOME` 不用改。
4. 点 `Install` → 等 1~2 分钟 → `Finish`。

> 📸 **截图点 ②③**：**本次走 winget 路线，这两张不存在**——报告里不要写你没做过的步骤。
> 以后若真走路线 B，再补截 `Custom Setup` 页面和完成界面。

---

## 4. 验证（最关键的一步，别跳）

**先关掉所有已打开的终端窗口，重新开一个**（环境变量是"进程启动时"读进来的，旧窗口读不到新配置），然后依次敲：

```powershell
java -version
javac -version
where.exe java
$env:JAVA_HOME
```

**本机实测输出**（2026-09-15，你的机器就是这个结果）：

```
openjdk version "25.0.4.1" 2026-08-18 LTS
OpenJDK Runtime Environment Microsoft-14951867 (build 25.0.4.1+1-LTS)
OpenJDK 64-Bit Server VM Microsoft-14951867 (build 25.0.4.1+1-LTS, mixed mode, sharing)
javac 25.0.4.1
C:\Program Files\Microsoft\jdk-25.0.4.101-hotspot\bin\java.exe
C:\Program Files\Microsoft\jdk-25.0.4.101-hotspot\
```

**对照判断：**

| 现象 | 含义 | 怎么办 |
|---|---|---|
| 两个都正常 | ✅ 装好了 | 直接进第 7 节 |
| `java` 正常，`javac` 报"不是内部或外部命令" | 装成了只有运行时的 JRE，或路径没配全 | 看第 5 节 |
| 两个都报"不是内部或外部命令" | 装了但 PATH 没生效 | 第 5 节（**先重开终端再试**） |
| `$env:JAVA_HOME` 是空的 | PATH 有、JAVA_HOME 没配 | 第 5 节 B 部分 |

> 💡 小知识一：`where.exe java` 用来查"命令行到底找到了哪个 java"。
> ⚠️ 小知识二：在 PowerShell 里直接敲 `where java` 会**失败**——PowerShell 把 `where` 认成了自己的命令 `Where-Object`。所以要写 `where.exe`（带后缀）。
> 💡 小知识三：`java -version` 的输出走的是 **stderr（标准错误）**，不是标准输出。所以在 PowerShell 里它可能显示成红色"错误"字样，**其实完全正常**；想做重定向时 `java -version > v.txt` 存出来是**空文件**，得写 `java -version 2> v.txt`。

> 📸 **截图点 ④**：终端里 `java -version` + `javac -version` 的输出。**任务1 最核心的图，一定要截。**

---

## 5. 没配好？手动配 PATH 和 JAVA_HOME

本机 JDK 实际在：`C:\Program Files\Microsoft\jdk-25.0.4.101-hotspot\`
（判断标准：这一层里直接有 `bin`、`lib`、`conf`、`include`）。确认一下：

```powershell
Get-ChildItem 'C:\Program Files\Microsoft' -Directory
```

### A. 把 `bin` 加进 PATH（图形界面，推荐新手）

1. `Win + R` → 输入 `sysdm.cpl` → 回车
2. 「高级」选项卡 → 右下角「环境变量(N)...」
3. **下半部分「系统变量」**里找到 `Path` → 双击 → 「新建」→ 粘贴 `C:\Program Files\Microsoft\jdk-25.0.4.101-hotspot\bin`
   （没管理员权限 / 只想给自己用，就改**上半部分「用户变量」**的 `Path`，效果对你一样）
4. 一路「确定」（**每一层都要点确定，直接叉掉窗口等于没改**）

### B. 新建 `JAVA_HOME`

同一窗口 → 「新建」→ 变量名 `JAVA_HOME`，值 `C:\Program Files\Microsoft\jdk-25.0.4.101-hotspot`
（**到哪里为止？到 `bin` 的上一层，末尾不带 `\bin`**）。

### C. 命令行一行搞定（改用户级环境变量，不需要管理员）

```powershell
setx JAVA_HOME "C:\Program Files\Microsoft\jdk-25.0.4.101-hotspot"
setx PATH "$env:JAVA_HOME\bin;$env:PATH"
```

⚠️ `setx` 的坑：对**已打开的窗口无效**（要重开终端），且 PATH 有 **1024 字符上限**，太长会被截断。所以图形界面更保险；用 `setx` 前先备份当前 PATH：

```powershell
(Get-ItemProperty HKCU:\Environment).Path > "$env:USERPROFILE\Desktop\PATH备份.txt"
```

> 📸 **截图点 ⑤**：环境变量窗口里能看到 `JAVA_HOME` 的值 + `Path` 里 JDK 的 `bin`。

---

## 6. 报错对照表（先查这里，再问人）

| 报错 / 现象 | 常见原因 | 解决 |
|---|---|---|
| `'java' 不是内部或外部命令` | PATH 没配 / **没重开终端** | 第 5 节 A；先重开终端再试 |
| `'javac' 不是内部或外部命令`，但 `java` 正常 | 装的是 JRE，或 PATH 里少了 JDK 的 bin | 装 JDK（不是 JRE） |
| `找不到或无法加载主类 HelloWorld` | 在错误目录执行，或类名与文件名不一致 | 先 `cd` 到 `.java` 所在目录；文件名必须叫 `HelloWorld.java` |
| `错误: 编码GBK的不可映射字符` | 源码有中文，javac 默认按 GBK 读文件 | `javac -encoding UTF-8 HelloWorld.java`，或源码先只写英文 |
| `java` 输出了别的版本（如 1.8） | 电脑上有多个 Java，PATH 里老的排在前面 | `where.exe java` 查出来，把新 JDK 的 bin 上移 |
| `UnsupportedClassVersionError ... class file version 69.0` | 用**老的** JDK 去跑 JDK 25 编译出的 `.class` | 编译和运行要用**同一个** JDK；IDEA 里统一 Project SDK |
| `java -version` 显示成红色 / 被当成错误 | 它本来就走 stderr（见第 4 节） | 正常现象，不用管 |
| 终端里中文变乱码 | 终端编码不是 UTF-8 | 运行前敲 `chcp 65001` |
| 双击 MSI 没反应 | 被安全软件拦了 | 看火绒的拦截记录；或右键「以管理员身份运行」 |

---

## 7. 装完立刻练一次：HelloWorld

`code` 文件夹里已放好 `HelloWorld.java`（**不是作业**，只用来验证环境）。在**终端**里：

```powershell
cd "D:\AAA study\school_\java\code"
javac HelloWorld.java      # 编译：生成 HelloWorld.class
dir HelloWorld.*           # 看一眼，多出来一个 .class 文件
java HelloWorld            # 运行：不要写 .class 后缀
```

**为什么 `java HelloWorld` 不带后缀？** 因为 `javac` 编译出的是 `HelloWorld.class`，而 `java` 要的是**类名**（类叫 `HelloWorld`），不是文件名。写成 `java HelloWorld.class` 反而报"找不到或无法加载主类"。

**这条链路就是你以后每次写作业的固定动作：**
```
HelloWorld.java  --javac 编译-->  HelloWorld.class  --java 运行-->  控制台输出
（你写的源码，人能看懂）        （字节码，JVM 能看懂）           （程序跑起来）
```

> 📸 **截图点 ⑥**（加分项）：`javac` + `java` 两条命令和输出结果。

---

## 8. 实验1 任务1 截图清单（按**实际做的**截）

| # | 截什么 | 本次是否需要 |
|---|---|---|
| ① | `winget install --id Microsoft.OpenJDK.25 -e` 命令 + 输出 | ✅ **要**（你实际用的方式） |
| ② | 安装向导 `Custom Setup` 页面 | ❌ 没走这条路，别写 |
| ③ | 安装完成界面 | ❌ 同上 |
| ④ | 环境变量窗口：`JAVA_HOME` + `Path` 里的 JDK `bin` | ✅ 要 |
| ⑤ | 终端 `java -version` + `javac -version` 输出 | ⭐ **必截，最核心** |
| ⑥ | `javac` + `java` 跑通 HelloWorld 的输出 | ✅ 建议截（加分） |

**报告"2. 实验环境"这样填**：
> 操作系统：Windows 11 24H2（64 位）；JDK：Microsoft Build of OpenJDK 25.0.4.1（LTS，安装位置 `C:\Program Files\Microsoft\jdk-25.0.4.101-hotspot\`）；开发工具：IntelliJ IDEA Community 2024.3.4.1。

**报告"3. 实验过程"写配置流程时的骨架**（照你自己做的写）：
`用 winget 安装 JDK 25 → 系统自动写入 PATH 与 JAVA_HOME → 重开终端 → java -version / javac -version 验证 → 编译运行 HelloWorld 验证全链路`。

---

## 9. ⚠️ 关于 IDEA 2024.3.4.1 和 JDK 25

老师给的解压版 IDEA 是 **2024.3.4.1**（2024 年底发布），而 **JDK 25 是 2025 年 9 月发布的**——IDEA 比 JDK 老。可能出现：

- IDEA 里选 JDK 25 时提示不支持的 SDK，或语言级别最高只能选到 23/24；
- 基础作业**通常**照样能编译运行（编译实际由 JDK 自带的 `javac` 执行），但我不敢打包票，得实际试。

**真遇到了怎么办**（按顺序）：
1. 先照老师文档用 IDEA 2024.3.4.1 试，能跑就不折腾；
2. 报错就打住，告诉我，我们换一个 JDK 给它用——**你机器上已经有现成的**（见下）；
3. 或者升到新版 IDEA Community（2025.2+ 完整支持 JDK 25）。

**好消息（2026-09-15 实机扫描）**：你这台机器上**早就躺着别的 JDK**了，是 Android Studio 自带的，你一直没注意：

- `C:\Program Files\Android\openjdk\jdk-21.0.8\` ← **JDK 21，IDEA 2024.3 完整支持**，可应急
- `C:\Program Files (x86)\Android\openjdk\jdk-17.0.14\` ← 老版 Android Studio 残留，**别用**

它们**都不在 PATH 里**，所以完全不干扰你的 JDK 25（`where.exe java` 只找得到 25——这就是 PATH 的作用）。但这两个跟着 Android Studio 走，升级/卸载它可能就没了，长期用建议正经装一个 Temurin / 微软版 JDK 21。

> 命令行那条路（第 7 节）**不受这个影响**，JDK 25 一定能跑——实验1 任务1 本来就只要求装好环境 + 截图。

---

## 10. 以后想卸载 / 换版本

- 卸载：设置 → 应用 → 已安装的应用 → 找 `Microsoft Build of OpenJDK 25` → 卸载；顺手把第 5 节配的 `JAVA_HOME` 和 PATH 那条删掉。
- 换版本：装新的即可，改 `JAVA_HOME` 指向新目录；PATH 里旧的 `bin` 记得删，否则又会"跑的是老版本"。

---

## 11. 它占多大空间？要不要搬到 D 盘？

**2026-09-15 实测：**

| 项目 | 大小 | 说明 |
|---|---|---|
| JDK 程序本体 | **373.1 MB**（497 个文件） | `lib` 200.6 MB + `bin` 87.9 MB + `jmods` 84.0 MB + 其余 0.6 MB |
| MSI 安装缓存 | **188.8 MB** | `C:\Windows\Installer\20fca03c.msi`，Windows 自己留的**修复/卸载用副本** |
| **C 盘合计** | **≈ 562 MB** | |
| C 盘现状 | 总 400 GB，**剩余 145.5 GB** | 562 MB ≈ 剩余空间的 **0.4%** |

**结论：完全不用搬。** 145 GB 的剩余空间里花掉 0.5 GB，属于"看不见"的量级。

**为什么不能自己把文件夹拖到 D 盘？**
MSI 安装 ≠ 复制一个文件夹。安装时系统里登记了三处：注册表的卸载信息、系统 `PATH`、`JAVA_HOME`。你手动搬走文件夹，这三处**仍指向老地址**，于是 `java` 命令立刻失效，控制面板里的卸载项也坏掉。**真要换位置，正确做法是"卸载 → 重新安装时选 D 盘路径"。**

**真要搬的步骤**（顺便对比一下"安装版 vs 解压版"两种软件形态）：
```powershell
winget uninstall --id Microsoft.OpenJDK.25 -e     # 1. 先卸载（含清掉 MSI 缓存）
# 2. 重新下载 MSI 双击安装，在向导的路径页把 C:\Program Files\Microsoft\... 改成 D:\Dev\jdk25
# 3. 装完按第 4 节重新验证
```
> 对照记忆：老师给的 **IDEA 是"解压版"**（绿色版，解压就能跑、随便搬）；**JDK 是"安装版"**（MSI 登记进系统）。两种形态各有用途，别混着理解。

**⚠️ 不要手动删 `C:\Windows\Installer` 里的东西**——那是 Windows Installer 的数据库，删了以后很多软件的卸载/修复/更新全会坏。等你哪天卸载 JDK，它会被自动清掉。

**进阶选项（不建议新手做）**：`jmods`（84 MB）只有 `jlink`/`jpackage` 打包自定义运行时才用得到，纯写作业删掉不影响 `javac`/`java`；但 MSI 修复时会还原，而且以后要用 `jlink` 就得重装。**为了 84 MB 不值当。**
