# Week06 Day05：GDB 与 CMake Debug 构建

## 今日目标

- [ ] 能用 `-DCMAKE_BUILD_TYPE=Debug` 配置单配置生成器的 Debug 构建。
- [ ] 能解释调试符号和优化级别为什么会影响调试体验。
- [ ] 能在 GDB 中使用 `break`、`run`、`next`、`step`、`print`、`continue` 和 `bt`。
- [ ] 能定位示例程序中的空指针解引用，并完成练习程序的空指针检查。

## 构建记录

```bash
# 粘贴你实际执行的配置、构建和运行命令，以及关键输出。
```

### Debug 构建说明

<!-- 用自己的话说明：Debug 构建保留了什么；高优化级别为什么会让单步调试和查看变量更困难。 -->

## GDB 命令记录

| 命令 | 实际输出或观察 | 作用 |
| --- | --- | --- |
| `break main` |  |  |
| `run` |  |  |
| `next` |  |  |
| `step` |  |  |
| `print target` |  |  |
| `continue` |  |  |
| `bt` |  |  |

## 错误定位

<!-- 写下：崩溃发生在哪个函数、哪一行；传入的指针值是什么；调用栈如何证明它从 main 进入该函数。 -->

## 练习结果

- [ ] `day05_exercise` 在 target 不存在时正常提示并退出。
- [ ] `day05_exercise` 在 target 存在时输出对应成绩。
- [ ] 已在 Ubuntu 的 Debug 构建中运行两个场景。

## 遇到的问题

<!-- 记录 CMake 或 GDB 中实际遇到的错误、自己的判断和最终解决方式。 -->
