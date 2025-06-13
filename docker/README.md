# Dockerfile

## 依赖文件

如果需要使用dockerfile生成镜像，需要如下的依赖：

+ verilator源码

+ riscv-gnu-toolchain源码

+ fltk源码

依赖源码下载方式请查看主页[BLKRv主页](https://blkrv.moeyuki.net/)

其目录结构如下：

```content
+ deps
    - verilator
    - fltk-1.4.3
    - riscv-gnu-toolchain
```

三者的源文件在一个文件夹中，并将该文件夹压缩为deps.tar文件

压缩命令如下：

`tar cf deps.tar deps/`

将deps.tar文件放在Dockerfile同级目录下，在dockerfile文件目录下执行构建

`sudo docker build -t blkrv .`