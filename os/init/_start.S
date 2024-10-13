
.extern desc_table syscall_table;

.text
.global _start,_gdt_search,_idt_search
_start:
    la a0,_gdt_search//syscall_table adrr
    csrrw zero,stvec,a0//syscall addr
    li a0,0
    li a1,0
    li a2,0
    li a3,0
    li a4,0
    li a5,0
    j main

_gdt_search:
    slli a7,a7,2
    la t0,syscall_table
    add a7,a7,t0
    lw a7,0(a7)
    li t0,0
    jalr ra,a7
    sret

_idt_search:
