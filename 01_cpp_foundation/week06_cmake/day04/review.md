# Week06 Day04：静态库和目标依赖

## 今日目标

- [ ] 能说明头文件、源文件、静态库和可执行文件之间的关系。
- [ ] 能解释 `add_library(... STATIC ...)` 的作用。
- [ ] 能解释 `target_link_libraries` 的作用。
- [ ] 能在 Ubuntu 中从干净的 `build-linux/` 目录完成构建。

## 知识复盘

### `add_library`

<!-- 用自己的话解释 score_lib 静态库由哪些文件组成。 -->
由 score_lib.cpp 和 score_lib.h文件组成
### `target_link_libraries`

<!-- 说明 day04_example/day04_exercise 为什么要链接 score_lib。 -->
因为这些源文件中使用了 score_lib静态库中的函数
### 头文件和源文件

<!-- 说明声明放在哪里、实现放在哪里，以及为什么调用方不应包含 .cpp。 -->
声明放在头文件，实现放在源文件，不知道
## 实践结果

<!-- 记录两个程序的实际输出。 -->

## 遇到的问题

<!-- 记录配置、链接或运行错误，以及解决方法。 -->
构建编译规则时 把实现文件夹的名字写错了 应该 rm -rf 文件夹   然后重新构建