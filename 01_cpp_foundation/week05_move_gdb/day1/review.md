## gdb调试
先打断点 然后跑 然后找bt中最先崩溃的栈 frame进去 info所有局部变量 next step进行排查

# 命令
-g  调试时 编译必须加这个 使得信息更完整
break 后有三种方式  +函数名 +行号 +if（条件断点）
run 启动程序
next step 都是继续  关键在于next不进入函数内部 step进入函数内部
print 打印变量
print *ptr&N 打印N个元素
display 每次暂停都自动显示
backtrace 看调用栈
frame N 切换栈帧
info locals 查看所有局部变量
ptype 查看变量类型
continue 运行到下一个断点
delete info breakpoints  断点管理
list 显示当前位置的源码
quit / q  退出
段错误应该在崩溃前拦下来（条件断点），不是等崩了再看
越界写入会破坏栈上的返回地址，导致 bt 无法正常显示调用链
看 bt 时，跳过系统库/标准库，找第一个自己的代码