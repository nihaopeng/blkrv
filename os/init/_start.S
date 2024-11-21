
.extern desc_table syscall_table,inter_table;

.text
.global _start,_gdt_search,_idt_search
_start:
    la x1,_gdt_search//syscall_table adrr
    csrrw zero,stvec,x1//syscall addr
    la x1,_idt_search
    csrrw zero,mtvec,x1
    csrrwi zero,mstatus,8
    li x1,0
    j main

_gdt_search:
    slli a7,a7,2
    la t0,syscall_table
    add a7,a7,t0
    lw a7,0(a7)
    li t0,0
    lui sp,0x10400
    lui s0,0x10400
    jalr ra,a7
    sret

_idt_search:
    csrrw a7,mcause,zero
    slli a7,a7,2
    la t0,inter_table
    add a7,a7,t0
    lw a7,0(a7)
    li t0,0
    lui sp,0x10600
    lui s0,0x10600
    jalr ra,a7
    mret
