# BLKRv
a platform for new learner

## environment
linux

[verilator](https://blog.csdn.net/m0_59161987/article/details/136761879)

make

[riscv-unknow-elf-gcc](https://blog.csdn.net/m0_59161987/article/details/136761879)(required march:rv32i, mabi:ilp32)

fltk

## usage
### compile
`cd rtl/`

`make` for command version

`make ENABLE_GPU=1` for gui version

### it's time to run!
`make run`

## for more information

https://gitee.com/c-nameless/blkrv-pre-design

## some details
### instructions
![](./img/instructions.png)
### hardware architecture
![](./img/arch.png)
### test result
![](./img/res.png)

## os
[syscall_func](./os/doc/syscall_func.md)
   