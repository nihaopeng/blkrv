.text
.global _start
_start:
    addi sp,sp,-12
    sw ra,4(sp)
    sw s0,8(sp)
    addi s0,sp,12
    jal ra,main
    li a7,9
    ecall
