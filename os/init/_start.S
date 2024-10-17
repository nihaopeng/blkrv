
.extern desc_table syscall_table,inter_table;

.text
.global _start,_gdt_search,_idt_search
_start:
    la a0,_gdt_search//syscall_table adrr
    csrrw zero,stvec,a0//syscall addr
    la a0,_idt_search
    csrrw zero,mtvec,a0
    csrrwi zero,mstatus,8
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
    csrrw a7,mcause,zero
    slli a7,a7,2
    la t0,inter_table
    add a7,a7,t0
    lw a7,0(a7)
    li t0,0
    jalr ra,a7
    mret
