# [gdb参考命令](https://www.cnblogs.com/soul-stone/p/6131069.html)

1、启动

1）程序的运行参数。

set args //指定运行时参数。（如：set args 10 20 30 ）

show args //查看设置好的运行参数。

 

2）运行环境

path //设定程序的运行路径。

show paths //查看程序的运行路径。

set environment varname[=value] //设置环境变量。如：set env USER=hchen

show environment [varname] //查看环境变量

 

3）工作目录

cd //相当于shell的cd命令。

pwd //显示当前的工作目录。

 

4）程序的输入输出

info terminal//显示程序用到的终端的模式

使用重定向空值程序输出。如run<outfile

tty命令可以指定写输入输出的终端设备。如tty/dev/ttyb

 

2、断点

(gdb)bmake_< TAB> // 支持联想功能

break

break +offset

break -offset

break filename:linenum

break filename:function

break if 

break xx thread

disable 5-8

enable

info break

delete [breakpoints] [range…]

watch //数据断点，表达式可以是一个变量或地址

 

3、运行

c/continue //恢复程序的运行直到程序结束，或下一个断点的到来。

s/step //单步跟踪，如果有函数调用，他会进入该函数

n/next //同样单步跟踪，如果有函数调用，他不会进入函数。

finish //运行程序，直到当前函数完成返回。

return //使用return命令取消当前函数的执行，并立即返回

 

 

4、堆栈信息

bt/backtrace //n是一个正整数，表示只打印栈顶上n层的栈信息。

f/frame //切换当前的栈

select-frame 对应于frame 命令。

up-silently 对应于up命令。

down-silently 对应于down命令。

info frame //打印出更为详细的当前栈层的信息

info args //打印出当前函数的参数名及其值。

info locals //打印出当前函数中所有局部变量及其值。

list //显示程序第linenum行周围的源程序。

list 1,20  //查看1到20行的代码

list //显示函数名为function的函数的源程序。

list //显示当前行后面的源程序。

 

 

5、变量

print/p //expr可以为变量或地址

--file:variable

--function::variable

int *array=(int*)malloc(len*sizeof(int))

p *[array@len](mailto:array@len) //len为查看长度

print x=4

set var width=47 //set用于修改gdb参数，建议带上var

 

6、自动显示

设置一些自动显示的变量，当程序停住时，或是在你单步跟踪时，这些变量会自动显示

display 

undisplay 

delete display 

disable display 

enable display 

info display

 

7、condition

condition 

修改断点号为bnum的停止条件为expression

condition 

清除断点号为bnum的停止条件。

还有一个比较特殊的维护命令ignore，可以指定程序运行时，忽略停止条件几次。

ignore 

表示忽略断点号为bnum的停止条件count次。

 

8、多线程

(gdb)set scheduler-locking on/off //锁定调度器来避开gdb根据优先权来改变当前线程

(gdb)show scheduler-locking  //确定当前模式

(gdb)set follow-fork-mode child //跟踪子进程

(gdb)set follow-fork-mode parent //跟踪父进程

(gdb)set follow-fork-mode ask //设置gdb在fork时询问跟踪哪一个进程

info threads //查看当前应用程序中所有的线程