.text
.global _start
_start:
    jal ra,main
    li a7,9
    ecall