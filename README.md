![logo](./doc/logo@498x280.png)

# BLKRv
![license](https://img.shields.io/badge/license-mit-blue) ![version](https://img.shields.io/badge/version-1.0-blue)

BLKRv是一个基于RISC-V架构的教学级计算机系统模拟平台，项目完整实现了CPU寄存器体系、虚拟内存管理、总线通信、自定义外设协议和中断处理，其创新性的虚拟抽象协议简化了外设交互，帮助学生理解计算机体系结构核心原理的同时为研究者提供灵活扩展空间。

## 环境要求

+ linux

+ [verilator](https://blog.csdn.net/m0_59161987/article/details/136761879)

+ make

+ [riscv-unknow-elf-gcc](https://blog.csdn.net/m0_59161987/article/details/136761879)(required march:rv32i, mabi:ilp32)

+ fltk [可选]

详细的环境配置请见
[BLKRv主页](https://blkrv.moeyuki.net/)

## 运行

+ `make` 仅命令行

+ `make clean&&make ENABLE_GPU=1` 使能图形化界面

+ `make vis file_path="./data.txt"` 启动可视化服务

## 设计方案

[blkrv-pre-design](https://gitee.com/c-nameless/blkrv-pre-design)

[技术报告](./doc/技术报告.pdf)

## 运行效果

+ cli

![cli](./doc/res.png)

+ gui

![gui](./doc/guiRes.png)

![guitri](./doc/guitri.png)

+ 可视化

![vis](./doc/vis.png)