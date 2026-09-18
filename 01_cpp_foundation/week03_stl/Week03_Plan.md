# 第 3 周学习计划：STL 基础（容器 + 算法）

时间：5.27-6.2  
对应总计划：第 3 周，STL 基础  
本周目标：能独立写出"学生信息管理 CLI"小项目，使用 STL 容器和算法实现增删改查、排序、查找。

---

## 一、本周必须掌握

### 1. vector

重点：
- `push_back`、`pop_back`。
- `size()`、`capacity()`、`empty()`。
- 下标访问 `v[i]` vs `v.at(i)`（后者会检查越界）。
- `clear()`、`resize()`。
- `vector` 是连续内存，和数组一样快。

你要能回答：
- `vector` 扩容时会做什么？
- `capacity()` 和 `size()` 的区别？
- 为什么遍历 `vector` 用下标比链表快？

### 2. string

重点：
- `string` 本质是 `vector<char>`，很多操作通用。
- `substr`、`find`、`replace`、`append`。
- `c_str()` 返回 C 风格字符串指针。
- `+` 和 `+=` 拼接。
- `to_string()`：int / double 转 string。

你要能回答：
- `string s = "hello"; s += " world";` 内部发生了什么？
- `s.find("abc")` 的返回值是什么？找不到返回什么？

### 3. map / unordered_map

重点：
- `map`：红黑树，键值对按键有序排列，操作 O(log n)。
- `unordered_map`：哈希表，键无序，操作平均 O(1)。
- `insert({key, value})` 或 `m[key] = value`。
- `m.find(key)` 返回迭代器，找不到返回 `m.end()`。
- `m.count(key)` 返回 0 或 1（`map`）/ 0 或 多个（`multimap`）。

你要能回答：
- `map` 和 `unordered_map` 的区别？什么时候用哪个？
- `m[key]` 和 `m.find(key)` 的区别？
- 为什么查学号要优先用 `unordered_map` 而不是 `map`？

### 4. 迭代器

重点：
- `begin()` / `end()`。
- 迭代器像"可以移动的指针"，指向容器中的位置。
- 正向迭代器、反向迭代器 `rbegin()`/`rend()`。
- `auto it = v.begin(); it != v.end(); ++it`。

你要能回答：
- 迭代器是什么？和指针有什么异同？
- 什么情况下迭代器会"失效"？

### 5. STL 算法

重点：
- `sort(v.begin(), v.end())`：默认升序。
- `sort(v.begin(), v.end(), greater<int>())`：降序。
- 自定义排序：传第三个参数是 lambda / 函数指针。
- `find(v.begin(), v.end(), value)`。
- `for_each`：对每个元素执行某个操作。
- `count_if`：统计满足条件的元素个数。

你要能回答：
- `sort` 的第三个参数有什么用？怎么写？
- `find` 找不到返回什么？

---

## 二、推荐学习资料

### 主线视频

**黑马程序员《C++教程从0到1入门编程》**（共 300+ 集）

主链接：https://www.bilibili.com/video/BV1et411b73Z/

本周具体章节：（注：P 数为近似值，以实际链接为准）

| 章节 | 内容 | 链接 | P 数 |
|------|------|------|------|
| string 容器 | 构造/赋值/拼接/查找/替换/比较/存取/插入/删除/子串 | https://www.bilibili.com/video/BV1et411b73Z/?p=191 | P191-P194 |
| vector 容器 | 构造/赋值/容量大小/插入删除/存取/互换/预留空间 | https://www.bilibili.com/video/BV1et411b73Z/?p=194 | P194-P196 |
| deque 容器 | 双端数组（了解一下就行） | https://www.bilibili.com/video/BV1et411b73Z/?p=197 | P197-P200 |
| list 容器 | 链表/反转/排序 | https://www.bilibili.com/video/BV1et411b73Z/?p=215 | P215-P222 |
| set/multiset 容器 | 集合/自动排序/不允许重复 | https://www.bilibili.com/video/BV1et411b73Z/?p=223 | P223-P225 |
| map/multimap 容器 | 键值对/红黑树/排序 | https://www.bilibili.com/video/BV1et411b73Z/?p=226 | P226-P230 |
| 函数对象 | 函数对象/谓词/内建函数对象 | https://www.bilibili.com/video/BV1et411b73Z/?p=236 | P236-P242 |
| STL 常用算法 | for_each/transform/find/sort/copy/replace/count/accumulate/fill/set_intersection 等 | https://www.bilibili.com/video/BV1et411b73Z/?p=243 | P243-P250 |

> **注意**：`unordered_map` 黑马视频中可能讲得很少或没讲，参考下面补充视频或 cppreference。

### 补充视频

- **C++ STL 标准模板库**：https://www.bilibili.com/video/BV1Wx4y1A7iE/
  - 重点看：容器概述（P1-P5）、迭代器（P6-P10）、常用算法（P11-P20）
- **unordered_map 补充**：搜索 "C++ unordered_map 教程" 或 直接看 cppreference

### 文档查阅

cppreference：
- `std::vector`、`std::string`、`std::map`、`std::unordered_map`
- `std::sort`、`std::find`、`std::for_each`、`std::count_if`

---

## 三、每日安排

### Day 1：vector 详解

学习内容：
- vector 的创建、增删、访问、扩容机制。

对应视频：
- vector 容器 P194-P196：https://www.bilibili.com/video/BV1et411b73Z/?p=194

代码练习：
- 创建一个 `vector<int>`，写入 100 个偶数，打印 `size()` 和 `capacity()`。
- 对比 `push_back` 前后的 `capacity` 变化，观察扩容规律。
- 测试 `v[i]` 和 `v.at(i)` 越界时的不同行为。

文件：
```text
week03/
  day1_vector.cpp
```

今日验收：
- 能说出 vector 扩容策略（VS 下 1.5 倍，GCC 下 2 倍）。
- 能说出 `size()` 和 `capacity()` 的区别。

### Day 2：string 和迭代器

学习内容：
- string 常用操作。
- 迭代器的概念和遍历方法。

对应视频：
- string 容器 P191-P194：https://www.bilibili.com/video/BV1et411b73Z/?p=191
- 补充视频：迭代器概念 https://www.bilibili.com/video/BV1Wx4y1A7iE/ （看迭代器章节）

代码练习：
- 用 `string` 完成：拼接、查找子串、替换、截取。
- 用正向迭代器遍历一个 `vector<int>`。
- 用反向迭代器倒序遍历。
- 用范围 for 遍历（语法糖，底层就是迭代器）。

文件：
```text
week03/
  day2_string_iterator.cpp
```

今日验收：
- 能解释迭代器和指针的异同。
- 能用至少 3 种方式遍历一个 vector。

### Day 3：map / unordered_map

学习内容：
- pair 和键值对。
- map 和 unordered_map 的 insert、find、遍历。

对应视频：
- set/multiset 容器 P223-P225：https://www.bilibili.com/video/BV1et411b73Z/?p=223
- map/multimap 容器 P226-P230：https://www.bilibili.com/video/BV1et411b73Z/?p=226
- unordered_map 黑马没细讲，看 cppreference：https://en.cppreference.com/w/cpp/container/unordered_map

代码练习：
- 用 `map<string, int>` 统计一段英文文本中每个单词出现的次数（单词频率统计）。
- 用 `unordered_map<int, string>` 实现"学号 → 姓名"的快速查找。
- 对比 map 遍历输出的顺序和 unordered_map 的差异。

文件：
```text
week03/
  day3_map.cpp
```

今日验收：
- 能说清楚 map 和 unordered_map 的区别。
- 能解释为什么学号查找用 unordered_map 更快。

### Day 4：STL 算法

学习内容：
- sort、find、for_each、count_if。
- lambda 表达式作为排序和筛选条件。

对应视频：
- 黑马 C++ STL 常用算法 P243-P250：https://www.bilibili.com/video/BV1et411b73Z/?p=243
  - for_each、transform：P243-P244
  - find 系列：P245-P246
  - sort、random_shuffle：P247-P248
  - copy、replace、accumulate：P249-P250

代码练习：
- 对 `vector<int>` 分别升序和降序排序。
- 对 `vector<pair<string, int>>`（姓名+成绩），按成绩降序排序（自定义 lambda）。
- 用 `find` 查找某个值。
- 用 `count_if` 统计成绩 >= 90 的人数。

文件：
```text
week03/
  day4_algorithm.cpp
```

今日验收：
- 能写出自定义排序的 lambda。
- 能用 `count_if` + lambda 实现条件统计。

### Day 5：值语义与迭代器失效

学习内容：
- 容器的值语义（存的是副本还是指针？）。
- 迭代器失效的常见场景。
- `vector` 扩容导致迭代器失效。

对应视频：
- vector 扩容与拷贝 P194-P196（复习）：https://www.bilibili.com/video/BV1et411b73Z/?p=194
- 迭代器失效原理建议搜索 CSDN 文章 "C++ 迭代器失效场景详解" 配合理解

代码练习：
- 展示 vector 存入对象时调用拷贝构造（打印日志验证）。
- 演示 vector `push_back` 导致扩容后，旧迭代器失效的情况。
- 对比存对象 vs 存指针 vs 存 `unique_ptr`。

文件：
```text
week03/
  day5_value_semantics.cpp
```

今日验收：
- 能解释"往 vector 存对象"和"存对象指针"的区别。
- 能说出至少 2 种导致迭代器失效的场景。

### Day 6：本周验收项目 —— 学生信息管理 CLI

---

## 验收项目需求清单

> **项目目录**：`week03/student_manager/`
>
> **把下面每一条当作一个需求来完成，做完一条勾一条。**

---

### 一、数据结构

**需求 1.1**：定义 `Student` 结构体/类

| 字段 | 类型 | 说明 |
|------|------|------|
| id | string | 学号，如 "2024001" |
| name | string | 姓名 |
| math | int | 数学成绩，0-100 |
| chinese | int | 语文成绩，0-100 |
| english | int | 英语成绩，0-100 |

**需求 1.2**：`Student` 提供 `total()` 方法，返回三科总分。  
**需求 1.3**：`Student` 提供 `average()` 方法，返回三科平均分（double）。

---

### 二、仓库层（StudentRepository）

**需求 2.1**：创建一个类 `StudentRepository`，负责存储和管理所有学生。

**需求 2.2**：内部数据用两个容器：

| 容器 | 类型 | 用途 |
|------|------|------|
| `students_` | `vector<Student>` | 按添加顺序保存所有学生，用于遍历、排序 |
| `index_` | `unordered_map<string, int>` | 学号 → 在 vector 中的下标，用于 O(1) 快速查找 |

> 为什么用 unordered_map？学号查找是最频繁的操作，O(1) 比 map 的 O(log n) 更快。

**需求 2.3**：`StudentRepository` 提供以下方法：

| 方法 | 参数 | 返回值 | 功能 |
|------|------|--------|------|
| `add(Student s)` | 学生对象 | bool | 添加学生，学号重复返回 false |
| `remove(string id)` | 学号 | bool | 删除学生，不存在返回 false |
| `update(string id, Student s)` | 学号 + 新数据 | bool | 更新学生信息，不存在返回 false |
| `find_by_id(string id)` | 学号 | Student* | 按学号查找，找不到返回 nullptr |
| `list_all()` | 无 | vector<Student> | 返回所有学生（副本） |
| `sort_by_total(bool desc)` | 是否降序 | vector<Student> | 按总分排序后返回 |
| `count()` | 无 | int | 返回学生总数 |

---

### 三、命令行界面（CLI）

**需求 3.1**：`main()` 中显示菜单，循环等待用户输入：

```
===== 学生成绩管理系统 =====
1. 添加学生
2. 删除学生
3. 修改学生
4. 按学号查询
5. 查看全部学生
6. 按总分排序查看
7. 统计信息
8. 退出
请选择（1-8）：
```

**需求 3.2**：每个菜单项的功能要求：

| 菜单 | 输入 | 输出 |
|------|------|------|
| 添加学生 | 学号、姓名、语数英成绩 | 成功/失败提示 |
| 删除学生 | 学号 | 成功/失败提示 |
| 修改学生 | 学号、新姓名、新成绩 | 成功/失败提示 |
| 按学号查询 | 学号 | 显示学生详细信息 + 总分 + 平均分 |
| 查看全部 | 无 | 列表显示所有学生（学号、姓名、总分） |
| 按总分排序 | 无 | 按总分降序排列后显示所有学生 |
| 统计信息 | 无 | 见需求 3.3 |

**需求 3.3**：统计信息菜单输出以下内容：

```
===== 统计信息 =====
学生总数：X
平均分最高的学生：姓名（学号），平均分 XX.X
语文平均分：XX.X
数学平均分：XX.X
英语平均分：XX.X
90分以上学生数：X人  （三科都 >= 90）
不及格学生数：X人    （任一科 < 60）
```

**需求 3.4**：成绩输入合法性检查——输入成绩不在 0-100 范围时，提示错误并要求重新输入。

---

### 四、STL 使用要求

以下写法必须出现在你的代码中：

| 要求 | 说明 |
|------|------|
| `vector` 存学生 | 用 `vector<Student>` 不要用数组 |
| `unordered_map` 做索引 | 学号 → 下标，实现 O(1) 查找 |
| `sort` + lambda | 按总分降序排序时，用 lambda 写比较规则 |
| `for_each` 或范围 for | 遍历学生列表时使用 |
| `count_if` + lambda | 统计"三科都 >= 90"的人数、"任一科 < 60"的人数 |
| `find_if` | 查找平均分最高的学生 |

---

### 五、文件结构

```
week03/
  student_manager/
    main.cpp
    Student.h
    Student.cpp
    StudentRepository.h
    StudentRepository.cpp
    README.md
```

### 六、README.md 内容

- 项目名称和功能简介
- 编译命令（g++）
- 运行方式
- 菜单功能截图说明
- 学到的主要知识点（vector、unordered_map、sort、lambda 等）

---

### Day 7：复盘和面试题

整理文件：
```text
week03/
  week03_note.md
```

必须用自己的话写清楚：
1. `vector` 扩容机制是什么？`size()` 和 `capacity()` 的区别？
2. `string` 的 `find` 返回什么？找不到返回什么？
3. `map` 和 `unordered_map` 的区别？各自适用场景？
4. 迭代器是什么？什么情况下会失效？
5. `sort` 的第三个参数有什么作用？怎么写？
6. `for_each` 和范围 for 的异同？
7. `count_if` 的用法？lambda 怎么写？
8. 为什么查学号用 `unordered_map` 而不是 `vector` 遍历？
9. 往 `vector` 里存对象和存指针各自有什么优缺点？
10. 如何用 STL 实现一个"查找容器中满足条件的第一个元素"？

本周提交：
- 至少 3 次 Git commit。
- 学生管理 CLI 程序能完整运行。
- README 有截图和编译说明。

---

## 四、本周不要踩的坑

- 不要在 `vector` 循环中 `push_back` 的同时用之前保存的迭代器（扩容会导致失效）。
- 不要混淆 `map` 和 `unordered_map` 的适用场景：需要有序就用 map，需要快速查找就用 unordered_map。
- 不要用 `m[id]` 来"判断是否存在"（不存在的 key 会被自动插入默认值）。用 `m.find(id) != m.end()` 或 `m.count(id)`。
- 不要忘记 `#include <algorithm>` 才能用 `sort`、`find`、`for_each`、`count_if`。
- 不要手写冒泡排序去对 `vector` 排序——既然学了 STL，直接用 `sort` + lambda。
