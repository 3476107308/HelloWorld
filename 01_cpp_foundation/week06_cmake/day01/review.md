# 第一次使用cmake构建
## cmake -S . -B build-linux
-S表示指定源码目录为当前目录    -B表示将生成的构建文件放在build-linux
## cmake --build build-linux
使用已经生成的构建文件进行编译 build-linux
## add_executable
添加需要编译的文件
## target_include_directories
用来规定去哪里找头文件
## 为什么源码和 build-linux要分开
将实现和产物分开 这样便于管理
## 补充理解
CMake 负责读取 CMakeLists.txt 并生成构建规则，真正执行编译的是 GCC。
如果修改了 CMakeLists.txt，需要重新执行 cmake -S . -B build-linux；
如果只修改源代码，通常直接执行 cmake --build build-linux 即可。

Windows 用于编写和保存源码，Ubuntu 用于构建、运行和调试。
build-linux 是构建产物目录，不应和源码一起上传或提交 Git。