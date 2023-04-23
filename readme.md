### 概述

```
高级语言--————》汇编语言------》机器语言--------》计算机
		汇编			 汇编				运行
```

### gcc和g++区别

- 编译阶段，g++会调用gcc，对于c++代码，两者都是等价的。但是因为gcc命令不能自动和C++程序使用的库链接，通常使用g++进行链接。
- gcc不会定义__cplusplus宏，而g++会。宏标志着编译器将会把代码按照c还是c++语法解释。
- 编译可以用gcc/g++，而链接可以用g++或者gcc -lstdc++

### GCC工作流程

![gcc工作流程](F:\chorme download\gcc工作流程.png)

| 后缀             | 描述                    | 后缀     | 描述           |
| ---------------- | ----------------------- | -------- | -------------- |
| .c               | C 源文件                | .s/.S    | 汇编语言源文件 |
| .C/.cc/.cxx/.cpp | C++ 源文件              | .o/.obj  | 目标文件       |
| .h               | C/C++ 头文件            | .a/.lib  | 静态库         |
| .i/.ii           | 经过预处理的 C/C++ 文件 | .so/.dll | 动态库         |

### gcc编译选项

|               gcc编译选项               |                      说明                      |
| :-------------------------------------: | :--------------------------------------------: |
|                   -E                    |         预处理指定的源文件，不进行编译         |
|                   -S                    |     预处理、编译指定源文件，但是不进行链接     |
|                   -c                    | 预处理、编译、汇编执行的源文件，但是不进行链接 |
| -o [file1] [file2] / [file2] -o [file1] |        将文件file2编译成可执行文件file1        |
|                   -I                    |               指定头文件包含路径               |
|                   -D                    |          在程序编译的时候，指定一个宏          |
|                   -w                    |               不生成任何警告信息               |
|                   -L                    |              指定链接库的包含路径              |
|                 -shared                 |                   创建动态库                   |
|                 -static                 |                  创建静态链接                  |
|                   -On                   |             优化，n:0~3。优化级别              |

预处理：

```shell
gcc test.c -E -o test.i
```

汇编

```shell
gcc test.i -S -o test.s
```

编译链接

```shell
gcc test.s -s -o test.o
```

注意：

```shell
gcc test.c -c -o test.o
直接使用-c命令，并不会生成可执行文件，因为没有链接
```

指定宏

```shell
gcc d.cpp -o d.o -DDEBUG	#指定DEBUG宏
    using -D, gcc d.cpp -o d.o -DDEBUG
    gcc d.cpp -o d.o

gcc d.cpp -o d.o #未指定宏
	gcc d.cpp -o d.o
```

警告信息

```shell
gcc d.cpp -o d.o -Wall#生成警告信息
d.cpp: In function ‘int main()’:
d.cpp:8:9: warning: unused variable ‘a’ [-Wunused-variable]
    8 |     int a = 10;
      |         ^

gcc d.cpp -o d.o -w	#不生成警告信息
```

### 静态库

静态库：gcc进行链接时，会把静态库中的代码打包到可执行程序中

- 命名规则

  - Linux：libxxx.a
    1. lib：前缀
    2. xxx：库的名字
    3. .a：后缀
  - Windows：libxxx.lib

- 静态库制作

  - gcc获得.o文件

  - 将.o文件打包，使用ar（archive）工具

    ```shell
    ar rcs libxxx.a xxx.o xxx.o
    r - 将文件插入备存文件中
    c - 建立备存文件
    s - 索引
    ```

- 实操

  ```shell
  文件目录
  .
  ├── include
  │   └── head.h
  ├── lib
  │   └── libcalc.a
  ├── main.c
  └── src
      ├── add.c
      ├── div.c
      ├── mult.c
      └── sub.c
  
  ```

  ```shell
  gcc main.c -o app
  main.c:2:10: fatal error: head.h: No such file or directory
      2 | #include "head.h"
        |          ^~~~~~~~
  compilation terminated.
  未找到头文件
  
  ```

  ```shell
  gcc main.c -o app -I ./include/
  /usr/bin/ld: /tmp/cccRypwH.o: in function `main':
  main.c:(.text+0x41): undefined reference to `add'
  /usr/bin/ld: main.c:(.text+0x66): undefined reference to `subtract'
  /usr/bin/ld: main.c:(.text+0x8b): undefined reference to `multiply'
  /usr/bin/ld: main.c:(.text+0xb0): undefined reference to `divide'
  collect2: error: ld returned 1 exit status
  加上指定头文件路径
  
  ```

  ```shell
  gcc main.c -o app -I ./include/ -l clac
  /usr/bin/ld: cannot find -lclac: No such file or directory
  collect2: error: ld returned 1 exit status
  加上静态库的名称，但是未指定库的位置
  不要使用libclac.c，只使用clac
  
  ```

  ```shell
  gcc main.c -o app -I ./include/ -l clac -L ./lib/
  加上指定静态库的路径，编译链接成功
  
  ```

### 动态库

动态库：在链接阶段没有被复制到程序中，而是在程序运行时由系统动态加载到内存中供程序调用。通过ldd(list dynamic dependencies)命令检查动态库依赖关系。  

- 命名规则

  - Linux：libxxx.so
    - lib：前缀
    - xxx：库的名字
    - .so：后缀
  - Windows：libxxx.dll

- 动态库的制作

  - gcc得到.o文件，得到和位置无关的代码

    ```shell
    gcc -c -fpic/-fPIC xxx.c xxxx.c
    
    ```

  - gcc得到动态库

    ```shell
    gcc -shared xxx.o xxxx.o -o libxxx.so
    
    ```

- 定位共享库文件

  - 系统加载可执行代码的时候，能知道其所依赖的库的名字，但是还要知道绝对路径。

  - 需要系统的动态载入器来获取该绝对路径

  - 对于elf格式的可执行文件，是ld-linux.so来完成的，它先后搜索elf文件的

    **DT_RPATH段**  ——》 **环境变量LD_LIBRARY_PATH**  ——》 **/etc/ld.so.cache文件列表**——》 **/lib/,/usr/lib目录** 找到库文件后将其载入内存

- 实操

  ```shell
  gcc -fpic -c *.c	#得到与位置无关的.o文件
  
  ```

  ```shell
  gcc -shared *.o -o libclac.so	#得到动态库
  
  ```

  ```shell
  tree	#查看文件目录结构
  .
  ├── include
  │   └── head.h
  ├── lib
  │   └── libclac.so
  ├── main.c
  └── src
      ├── add.c
      ├── div.c
      ├── makefile
      ├── mult.c
      └── sub.c
  
  ```

  ```shell
  gcc main.c -o app	#未找到头文件
  main.c:2:10: fatal error: head.h: No such file or directory
      2 | #include "head.h"
        |          ^~~~~~~~
  compilation terminated.
  
  ```

  ```shell
  gcc main.c -o app -I ./include/	#头文件定义了函数，但是未找到函数实现部分
  /usr/bin/ld: /tmp/ccVsKcmF.o: in function `main':
  main.c:(.text+0x41): undefined reference to `add'
  /usr/bin/ld: main.c:(.text+0x66): undefined reference to `subtract'
  /usr/bin/ld: main.c:(.text+0x8b): undefined reference to `multiply'
  /usr/bin/ld: main.c:(.text+0xb0): undefined reference to `divide'
  
  ```

  ```shell
  gcc main.c -o app -I ./include/ -l clac -L ./lib/	#指定动态库名称和位置
  
  ```

  ```shell
   ./app 	#执行文件
  ./app: error while loading shared libraries: libclac.so: cannot open shared object file: No such file or directory
  
  ```

  ```shell
  ldd app	#查看app所依赖的动态库
          linux-vdso.so.1 (0x00007ffc609f8000)
          libclac.so => not found
          libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007fe074e00000)
          /lib64/ld-linux-x86-64.so.2 (0x00007fe075187000)
  
  ```

  ```shell
  export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/hxk/C++Project/gcc_test/dynamic/library/lib
  #导入环境变量，但是此种方法只能使用一次，终端重置之后，不能再次使用
  
  ```

  ```shell
  echo $LD_LIBRARY_PATH	#查看LD_LIBRARY_PATH环境变量
  :/home/hxk/C++Project/gcc_test/dynamic/library/lib
  
  ```

  ```shell
  ldd app	#再次查看app所依赖的动态库
          linux-vdso.so.1 (0x00007ffe35c49000)
          libclac.so => /home/hxk/C++Project/gcc_test/dynamic/library/lib/libclac.so (0x00007fb692090000)
          libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007fb691e00000)
          /lib64/ld-linux-x86-64.so.2 (0x00007fb69209c000)
  
  ```

  ```shell
  ./app #执行程序，成功
  a = 20, b = 12
  a + b = 32
  a - b = 8
  a * b = 240
  a / b = 1.666667
  
  ```

  第一种解决方法——用户级别

  ```shell
  vim .bashrc	#在.bashrc中添加
  export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/hxk/C++Project/gcc_test/dynamic/library/lib
  
  ```

  ```
  source .bashrc	#使生效
  
  ```

  第二种解决方法——系统级别

  ```shell
  sudo vim /etc/ld.so.conf	#编辑文件
  /home/hxk/C++Project/gcc_test/dynamic/library/lib	#添加路径
  sudo ldconfig	#更新配置
  
  ```

  ```shell
  ldd app #查看动态库位置
          linux-vdso.so.1 (0x00007ffc0ff50000)
          libclac.so => /home/hxk/C++Project/gcc_test/dynamic/library/lib/libclac.so (0x00007fcf21480000)
          libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007fcf21200000)
          /lib64/ld-linux-x86-64.so.2 (0x00007fcf2149b000)
  
  ```

  第三种方式

  ```shell
  /lib/	/usr/lib	#将动态库放在以上两个目录下
  
  ```



### 静态库和动态库的区别

静态库和动态库的区别来自链接阶段如何处理，链接成可执行程序。静态库直接链接成可执行程序；动态库在链接时会保存相应信息，等到程序运行时再加载进内存。

#### 静态库

- 优点
  - 静态库被打包到应用程序中加载速度快
  - 发布程序无需提供静态库
- 缺点
  - 消耗系统资源，浪费内存
  - 更新、部署、发布麻烦

#### 动态库

- 优点
  - 可以实现进程间资源共享
  - 更新、部署、发布简单
  - 可以控制何时加载动态库
- 缺点
  - 加载速度比静态库慢
  - 发布程序时需要提供依赖的动态库