# Week06 Day05: GDB 与 CMake Debug 构建

本目录延续 Day04 的“库目标 + 可执行目标”结构。`score_debug` 负责查找成绩；两个程序通过 `target_link_libraries` 链接它。

`day05_example` 中有一个刻意保留的空指针解引用。它不是生产代码写法，只用于练习让 GDB 定位：未找到 `100` 时，`find_score` 返回 `nullptr`，随后 `print_found_score` 解引用该指针而崩溃。

## Ubuntu Debug 构建

在本目录执行：

```bash
rm -rf build-linux
cmake -S . -B build-linux -DCMAKE_BUILD_TYPE=Debug
cmake --build build-linux -j2
./build-linux/day05_example
```

Debug 构建通常会保留调试符号，并避免高优化级别对变量和执行顺序的重排。可用下面命令确认可执行文件带有调试信息：

```bash
file build-linux/day05_example
```

## GDB 示例程序练习

```bash
gdb ./build-linux/day05_example
```

在 GDB 中依次尝试以下命令，并将你实际看到的输出记录到 `review.md`：

```gdb
break main
run
next
next
step
print target
continue
bt
```

提示：`step` 适合进入 `find_score` 观察循环和返回值；程序因空指针崩溃后执行 `bt`，可以看到 `print_found_score` 和 `main` 的调用关系。断点行号会随源码变化，先用 `list` 或 `info breakpoints` 确认即可。

## 独立练习

完成 `src/exercise_main.cpp` 的 TODO。要求是先在 GDB 看到空指针，再自己添加检查，使程序在“找到”和“找不到”两种情况下都正常退出。

不要修改 `score_debug.cpp` 来绕过练习；调用方应根据接口约定检查可能为空的返回指针。
