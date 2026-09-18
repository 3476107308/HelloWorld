# 学习约定提示词

## 关于我

- 大一学生，正在按 35 周 C++/Qt 实习计划学习
- GitHub 仓库：https://github.com/3476107308/cpp-study.git
- 开发环境：Windows 11，VS Code + GCC (MSYS2, C++17)
- 当前进度按 `newPlan.md` 推进

## 每日代码组织方式

每个 day 文件夹放两个 .cpp 文件：

| 文件 | 作用 |
|------|------|
| `example.cpp` | 完整教学实例，带详细中文注释，用来学习 |
| `exercise.cpp` | 练习需求（TODO 骨架），我自己写，用来检验能否脱离参考独立写出代码 |

## 代码风格要求

- 教学代码：每行关键逻辑都要有中文注释，解释 WHY
- 练习代码：只给需求清单（`// TODO: xxx`），不给完整代码
- 编码：UTF-8，`SetConsoleOutputCP(CP_UTF8)` 解决中文乱码
- 每个知识点拆成独立函数（如 `lesson1_值语义()` / `test_值语义()`）
- 使用 `using std::xxx;` 风格，不用 `using namespace std;`

## review.md 写作规范

每天完成 exercise.cpp 后，写一个 `review.md` 记录当天学到的知识点。

**内容要求：**

- 用自己的话写，不要复制 example.cpp 的注释
- 每个知识点写 1-3 句话，讲清楚核心思想和为什么重要
- 不需要写代码，只写理解和总结
- 参考当日验收标准，确保能回答"你要能回答"里列出的问题

**格式参考（按当天的知识点拆成小节）：**

```markdown
# Day X: 标题

## 知识点 1
一两句话描述核心概念。
解决的问题是什么，为什么需要它。

## 知识点 2
...

## 易错点（可选）
今天踩过的坑，提醒自己以后注意。
```

**目的：**

- 一周结束时，把这些 review 汇总起来就是 week_note.md
- 面试前翻出来快速回顾，比看代码快得多

## 每周验收

- 验收项目只提需求清单（需求 1.1、1.2...），我自己完成后你帮我检查
- 需求要具体到字段名、方法签名、返回值类型

## 学习资料

- 每份 example.cpp 顶部标注对应的知识点和学习周次
- 教学方式：Claude 生成 example.cpp（详细中文注释）+ exercise.cpp（需求骨架），独立完成后检查
- 补充视频（课后加深理解）：
  - 爱编程的大丙 C++11 新特性详解、Linux 基础、CMake、Qt5 等 https://space.bilibili.com/147020887
  - simplesoft GDB 调试合集 https://space.bilibili.com/391667478/lists/8218249
- 文档：https://en.cppreference.com/w/
- 大丙博客：https://subingwen.cn

## 我和你的协作方式

- 我看完 example.cpp 学习后，自己写 exercise.cpp
- 写完后你帮我检查，告诉我哪里有问题、怎么改
- 语法错误你只指出位置，让我自己动手修改
- 支持中文交流
