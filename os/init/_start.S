
.extern desc_table syscall_table,inter_table;

.text
.global _start,_gdt_search,_idt_search
_start:
    la x1,_gdt_search//syscall_table adrr
    csrrw zero,stvec,x1//syscall addr
    la x1,_idt_search
    csrrw zero,mtvec,x1
    csrrwi zero,mstatus,8//中断使能
    li x1,0
    j main

.macro save_ctx
    sw x3,-20(s0)//保存上下文
    sw x4,-24(s0)
    sw x5,-28(s0)
    sw x6,-32(s0)
    sw x7,-36(s0)
    sw x9,-44(s0)
    sw x10,-48(s0)
    sw x11,-52(s0)
    sw x12,-56(s0)
    sw x13,-60(s0)
    sw x14,-64(s0)
    sw x15,-68(s0)
    sw x16,-72(s0)
    sw x17,-76(s0)
    sw x18,-80(s0)
    sw x19,-84(s0)
    sw x20,-88(s0)
    sw x21,-92(s0)
    sw x22,-96(s0)
    sw x23,-100(s0)
    sw x24,-104(s0)
    sw x25,-108(s0)
    sw x26,-112(s0)
    sw x27,-116(s0)
    sw x28,-120(s0)
    sw x29,-124(s0)
    sw x30,-128(s0)
    sw x31,-132(s0)
.endm

.macro recover_ctx
    lw x3,-20(s0)//恢复上下文
    lw x4,-24(s0)
    lw x5,-28(s0)
    lw x6,-32(s0)
    lw x7,-36(s0)
    lw x9,-44(s0)
    lw x10,-48(s0)
    lw x11,-52(s0)
    lw x12,-56(s0)
    lw x13,-60(s0)
    lw x14,-64(s0)
    lw x15,-68(s0)
    lw x16,-72(s0)
    lw x17,-76(s0)
    lw x18,-80(s0)
    lw x19,-84(s0)
    lw x20,-88(s0)
    lw x21,-92(s0)
    lw x22,-96(s0)
    lw x23,-100(s0)
    lw x24,-104(s0)
    lw x25,-108(s0)
    lw x26,-112(s0)
    lw x27,-116(s0)
    lw x28,-120(s0)
    lw x29,-124(s0)
    lw x30,-128(s0)
    lw x31,-132(s0)
.endm

_gdt_search:
    addi sp,sp,-132
    sw ra,128(sp)
    sw s0,124(sp)
    addi s0,sp,132
    save_ctx//保存上下文
    slli a7,a7,2
    la a4,syscall_table//调用表基址
    add a7,a7,a4
    lw a7,0(a7)
    # lui sp,0x00500//设置栈指针
    # lui s0,0x00500
    jalr a7
    recover_ctx
    lw ra,128(sp)
    lw s0,124(sp)
    addi sp,sp,132
    sret

_idt_search:
    addi sp,sp,-132
    sw ra,128(sp)
    sw s0,124(sp)
    addi s0,sp,132
    save_ctx//保存上下文
    csrrw a7,mcause,zero
    slli a7,a7,2
    la a4,inter_table//中断表基址
    add a7,a7,a4
    lw a7,0(a7)
    # lui sp,0x00700//设置栈指针
    # lui s0,0x00700
    jalr a7
    recover_ctx
    lw ra,128(sp)
    lw s0,124(sp)
    addi sp,sp,132
    mret
