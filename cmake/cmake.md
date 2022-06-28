# cmake使用、

## 1、cmake文件主要内容

### 1.1 最低版本要求

```
cmake_minimum_required(VERSION 3.5.1)，如果用到了低版本不支持的命令，用这个进行提示升级，非必要。
```



### 1.2 项目名称

```c
project(demo)命令为非必须，如果没有指定，cmake仍会生成一个默认的项目名，以及工程名对应的PROJECT_NAME、CMAKE_PROJECT_NAME、PROJECT_SOURCE_DIR、PROJECT_BINARY_DIR等，但是VERSION、DESCRIPTION、HOMEPAGEURL不会被赋值，LANGUAGES不指定会存在一个默认语言C和CXX。
```



### 1.3 添加头文件路径

```
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

linux还可以：
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${CMAKE_CURRENT_SOURCE_DIR}")
```



### 1.4 添加源文件

#### 1.4.1 指定添加哪些源文件

```
#设置一个通用路径变量
SET(SRCS src)

#生成库
add_library(demo ${SRCS}/file_parse_h264.c ${SRCS}/share_memory.c)

#生成可执行文件
ADD_EXECUTABLE(demo ${SRCS}/file_parse_h264.c ${SRCS}/share_memory.c)
```



#### 1.4.2 添加目录（默认添加所有c和cpp文件，无法选择）

```
# 搜索当前目录下的所有源文件并保存在SRC_LIST变量中
aux_source_directory(. SRC_LIST) 

#搜索指定目录下的所有源文件并保存在SRC_LIST变量中
aux_source_directory(src SRC_LIST)

#生成库
add_library(demo ${SRC_LIST})

#生成可执行文件
ADD_EXECUTABLE(demo ${SRC_LIST})
```



#### 1.4.3 自定义搜索（可自定义搜索文件类型）

```
#方式一
file(GLOB SRC_LIST "*.cpp" "protocol/*.cpp")
add_library(demo ${SRC_LIST})

#方式二 非递归搜索（只搜索指定目录）
file(GLOB SRC_LIST "*.cpp")
file(GLOB SRC_PROTOCOL_LIST "protocol/*.cpp")
add_library(demo ${SRC_LIST} ${SRC_PROTOCOL_LIST})

#方式三 递归搜索（可以搜索子目录）
file(GLOB_RECURSE SRC_LIST "*.cpp") 
add_library(demo ${SRC_LIST} ${SRC_PROTOCOL_LIST})
```



### 1.5 添加库文件路径

```
link_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/libs
)
```



### 1.6 添加链接的库

```
#方式一 指定添加
##以变量形式指定添加
set(LIBS lib/demo-lib lib/demo1-lib.a lib/demo2-lib.so)
target_link_libraries(demo ${LIBS})

##直接添加
target_link_libraries(demo lib/demo-lib lib/demo1-lib.a lib/demo2-lib.so)

#方式二 指定路径搜索库添加
find_library(LIBS demo-lib ../common/Debug)
target_link_libraries(demo ${LIBS})
```



## 2、条件控制

### 2.1 if…elseif…else…endif

逻辑判断和比较：
if (expression)：expression 不为空（0,N,NO,OFF,FALSE,NOTFOUND）时为真
if (not exp)：与上面相反
if (var1 AND var2)
if (var1 OR var2)
if (COMMAND cmd)：如果 cmd 确实是命令并可调用为真
if (EXISTS dir) if (EXISTS file)：如果目录或文件存在为真
if (file1 IS_NEWER_THAN file2)：当 file1 比 file2 新，或 file1/file2 中有一个不存在时为真，文件名需使用全路径
if (IS_DIRECTORY dir)：当 dir 是目录时为真
if (DEFINED var)：如果变量被定义为真
if (var MATCHES regex)：给定的变量或者字符串能够匹配正则表达式 regex 时为真，此处 var 可以用 var 名，也可以用 ${var}
if (string MATCHES regex)

数字比较：
if (variable LESS number)：LESS 小于
if (string LESS number)
if (variable GREATER number)：GREATER 大于
if (string GREATER number)
if (variable EQUAL number)：EQUAL 等于
if (string EQUAL number)

字母表顺序比较：
if (variable STRLESS string)
if (string STRLESS string)
if (variable STRGREATER string)
if (string STRGREATER string)
if (variable STREQUAL string)
if (string STREQUAL string)

## 3、常用变量

### 3.1 预定义变量

PROJECT_SOURCE_DIR：工程的根目录
PROJECT_BINARY_DIR：运行 cmake 命令的目录，通常是 ${PROJECT_SOURCE_DIR}/build
PROJECT_NAME：返回通过 project 命令定义的项目名称
CMAKE_CURRENT_SOURCE_DIR：当前处理的 CMakeLists.txt 所在的路径
CMAKE_CURRENT_BINARY_DIR：target 编译目录
CMAKE_CURRENT_LIST_DIR：CMakeLists.txt 的完整路径
CMAKE_CURRENT_LIST_LINE：当前所在的行
CMAKE_MODULE_PATH：定义自己的 cmake 模块所在的路径，SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)，然后可以用INCLUDE命令来调用自己的模块
EXECUTABLE_OUTPUT_PATH：重新定义目标二进制可执行文件的存放位置
LIBRARY_OUTPUT_PATH：重新定义目标链接库文件的存放位置

### 3.2 环境变量

```
#使用环境变量
$ENV{Name}

#写入环境变量
set(ENV{Name} value) # 这里没有“$”符号
```



### 3.3 系统信息

```
CMAKE_MAJOR_VERSION：cmake 主版本号，比如 3.4.1 中的 3
CMAKE_MINOR_VERSION：cmake 次版本号，比如 3.4.1 中的 4
CMAKE_PATCH_VERSION：cmake 补丁等级，比如 3.4.1 中的 1
CMAKE_SYSTEM：系统名称，比如 Linux-2.6.22
CMAKE_SYSTEM_NAME：不包含版本的系统名，比如 Linux
CMAKE_SYSTEM_VERSION：系统版本，比如 2.6.22
CMAKE_SYSTEM_PROCESSOR：处理器名称，比如 i686
UNIX：在所有的类 UNIX 平台下该值为 TRUE，包括 OS X 和 cygwin
WIN32：在所有的 win32 平台下该值为 TRUE，包括 cygwin
```



## 4、主要开关选项

```
BUILD_SHARED_LIBS：这个开关用来控制默认的库编译方式，如果不进行设置，使用 add_library 又没有指定库类型的情况下，默认编译生成的库都是静态库。如果 set(BUILD_SHARED_LIBS ON) 后，默认生成的为动态库
CMAKE_C_FLAGS：设置 C 编译选项，也可以通过指令 add_definitions()添加
CMAKE_CXX_FLAGS：设置 C++ 编译选项，也可以通过指令 add_definitions()添加
#例如
add_definitions(-DENABLE_DEBUG -DABC) # 参数之间用空格分隔
```



## 5、打印信息

```
message(${PROJECT_SOURCE_DIR})
message("build with debug mode")
message(WARNING "this is warnning message")
message(FATAL_ERROR "this build has many error") # FATAL_ERROR 会导致编译失败
```



版权声明：本文为CSDN博主「阿飞__」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/afei__/article/details/81201039