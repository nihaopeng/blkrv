.text
.global _start
_start:
    addi sp,sp,-12
    sw ra,4(sp)
    sw s0,8(sp)
    addi s0,sp,12//以上为栈帧操作，进入_start之前，sp应被保存然后修改
    jal ra,main//a0-7默认转存不进行入栈操作，
    li a7,9
    ecall