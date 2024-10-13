# BLKRv
a platform for new learner

## environment
linux

verilator(https://blog.csdn.net/m0_59161987/article/details/136761879)

make

riscv-unknow-elf-gcc(tutorial:https://blog.csdn.net/m0_59161987/article/details/136761879)(required march:rv32i, mabi:ilp32)

## usage
### compile os
`cd os/`

`make`
### load os into flash
`cd rtl/utils/`

`./load_mem.sh`
### compile verilog and run
`cd rtl/`

`make`

## some details
### instructions
![](./preDesign/基础指令测试/instructions.png)
### hardware architecture
![](./preDesign/arch.png)
### test result
![](./preDesign/res.png)

