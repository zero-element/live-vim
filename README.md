# live-vim

## 项目结构

```
.
├── CMakeLists.txt
├── README.md
└── src
    ├── CMakeLists.txt
    ├── client              //客户端
    │   ├── CMakeLists.txt
    │   ├── buffer.c        //编辑区实现
    │   ├── buffer.h
    │   ├── editor.c        //编辑器实现
    │   ├── editor.h
    │   └── main.c          //客户端主程序
    ├── common              //公共基础库代码
    │   ├── CMakeLists.txt
    │   ├── dstring.c       //简易动态字符串实现
    │   ├── dstring.h
    │   ├── log.h           //服务端日志库
    │   ├── networking.c    //网络库
    │   ├── networking.h
    │   ├── thread.c        //线程库
    │   ├── thread.h
    │   ├── utils.c         //辅助函数
    │   └── utils.h
    └── server              //服务端
        ├── CMakeLists.txt
        └── main.c          //服务端主程序
```

## 编译方式

### VS Code

安装CMake Tools插件，底部状态栏点击`运行`按钮

### 命令行

首先确保自己处于项目根文件夹下

#### 客户端
```shell
mkdir build
cd build && cmake .. && cd ..
cmake --build ./build --target client
```

#### 服务端
```shell
mkdir build
cd build && cmake .. && cd ..
cmake --build ./build --target server
```

编译成功之后，二进制文件将输出到./build/bin/