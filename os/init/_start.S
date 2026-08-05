
.extern desc_table syscall_table,inter_table;

.text
.global _start,_gdt_search,_idt_search
_start:
    la x1,_gdt_search//syscall_table adrr
    csrw stvec,x1//syscall addr
    la x1,_idt_search
    csrw mtvec,x1
    csrrwi zero,mstatus,8//中断使能
    li x1,0
    j main

.macro save_ctx
    sw x2,-16(s0)
    sw x3,-20(s0)//保存上下文
    sw x4,-24(s0)
    sw x5,-28(s0)
    sw x6,-32(s0)
    sw x7,-36(s0)
    sw x9,-44(s0)
    # sw x10,-48(s0)//a0返回值
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
    # lw x2,-16(s0)//恢复上下文
    lw x3,-20(s0)
    lw x4,-24(s0)
    lw x5,-28(s0)
    lw x6,-32(s0)
    lw x7,-36(s0)
    lw x9,-44(s0)
    # lw x10,-48(s0)//a0
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

.macro _vir2phy
    csrr t0, 0x181             # 读取satp寄存器到t0
    lui t1, 0xfffff            # 构造掩码0xfffff000
    and t0, t0, t1             # 提取页表基址(satp & 0xfffff000)
    mv t2, sp                  # 将sp的值复制到t2
    srli t3, t2, 22            # 提取VPN[1] (高10位)
    slli t3, t3, 2             # 计算一级页表偏移(VPN[1] * 4)
    add t3, t0, t3             # 一级页表项地址
    lw t4, 0(t3)               # 加载一级页表项到t4
    lui t1, 0xfffff            # 重新加载PPN掩码
    and t4, t4, t1             # 提取二级页表基址(PTE1 & 0xfffff000)
    lui t5, 0x3ff              # 构造掩码0x003ff000
    and t3, t2, t5             # 提取中间VPN[0]部分
    srli t3, t3, 12            # 右移得到VPN[0]
    slli t3, t3, 2             # 计算二级页表偏移(VPN[0] * 4)
    add t3, t4, t3             # 二级页表项地址
    lw t4, 0(t3)               # 加载二级页表项到t4
    and t4, t4, t1             # 提取物理页号(PTE2 & 0xfffff000)
    li t5, 0xfff               # 构造页内偏移掩码t5 = 0x00000fff
    and t3, t2, t5             # 提取页内偏移(sp & 0xfff)
    add sp, t4, t3             # 组合物理地址到t6
.endm

_gdt_search:
    _vir2phy//转换后的物理sp地址存储在t6中，保存虚拟地址sp后，sp赋值为物理地址
    slli a7,a7,2
    la t1,syscall_table//调用表基址
    add a7,a7,t1
    lw a7,0(a7)
    jalr ra,a7
    sret

_idt_search:
    # 固定中断栈 (物理地址, satp=0, identity mapping)
    li sp,0x00300000
    addi sp,sp,-12

    csrrw t0, mepc, zero
    sw t0, 0(sp)                # [sp+0] = mepc
    csrrw t0, mcause, zero
    sw t0, 4(sp)                # [sp+4] = mcause
    sw ra, 8(sp)                # [sp+8] = ra

    # 分发中断: mcause 索引 inter_table
    slli t0, t0, 2
    la t1, inter_table
    add t0, t0, t1
    lw t0, 0(t0)

    jalr ra, t0

_idt_done:
    # 恢复 (handler/调度器可能已修改栈上的值)
    lw t0, 0(sp)
    csrrw zero, mepc, t0
    lw ra, 8(sp)

    addi sp, sp, 12
    mret
