#below is offset
rec_addr_prog_head=0x28
rec_size_prog_head=0x2a
rec_num_prog_head=0x2c
rec_addr_start=0x18

.text
.global enter_prog
.global save_contxt
.global recover_contxt

save_contxt:
    sw x1,4(a0)
    sw x2,8(a0)
    sw x3,12(a0)
    sw x4,16(a0)
    sw x5,20(a0)
    sw x6,24(a0)
    sw x7,28(a0)
    sw x8,32(a0)
    sw x9,36(a0)
    sw x10,40(a0)
    sw x11,44(a0)
    sw x12,48(a0)
    sw x13,52(a0)
    sw x14,56(a0)
    sw x15,60(a0)
    sw x16,64(a0)
    sw x17,68(a0)
    sw x18,72(a0)
    sw x19,76(a0)
    sw x20,80(a0)
    sw x21,84(a0)
    sw x22,88(a0)
    sw x23,92(a0)
    sw x24,96(a0)
    sw x25,100(a0)
    sw x26,104(a0)
    sw x27,108(a0)
    sw x28,112(a0)
    sw x29,116(a0)
    sw x30,120(a0)
    sw x31,124(a0)
    #csr reg save
    csrrw a4,mepc,zero
    sw a4,0(a1)
    csrrw a4,mcause,zero
    sw a4,4(a1)
    csrrw a4,mstatus,zero
    sw a4,8(a1)
    csrrw a4,satp,zero
    sw a4,12(a1)
    csrrw a4,sepc,zero
    sw a4,16(a1)
    #pc save
    sw a3,0(a2)
    ret#不涉及程序调用，无需进行ra保存

recover_contxt:
    #csr reg save
    lw a4,0(a1)
    csrrw zero,mepc,a4
    lw a4,4(a1)
    csrrw zero,mcause,a4
    lw a4,8(a1)
    csrrw zero,mstatus,a4
    lw a4,12(a1)
    csrrw zero,satp,a4
    lw a4,16(a1)
    csrrw zero,sepc,a4
    #general reg save
    lw x1,4(a0)
    lw x2,8(a0)
    lw x3,12(a0)
    lw x4,16(a0)
    lw x5,20(a0)
    lw x6,24(a0)
    lw x7,28(a0)
    lw x8,32(a0)
    lw x9,36(a0)
    lw x10,40(a0)
    lw x11,44(a0)
    lw x12,48(a0)
    lw x13,52(a0)
    lw x14,56(a0)
    lw x15,60(a0)
    lw x16,64(a0)
    lw x17,68(a0)
    lw x18,72(a0)
    lw x19,76(a0)
    lw x20,80(a0)
    lw x21,84(a0)
    lw x22,88(a0)
    lw x23,92(a0)
    lw x24,96(a0)
    lw x25,100(a0)
    lw x26,104(a0)
    lw x27,108(a0)
    lw x28,112(a0)
    lw x29,116(a0)
    lw x30,120(a0)
    lw x31,124(a0)
    #pc save
    lw ra,0(a2)
    ret#直接回到进程中断处

enter_prog:
    mv a1,a0
    li sp,0x00800000
    li s0,0x00800000
    la ra,exit
    addi a0,a0,rec_addr_start#记录程序main地址的地址
    lw a0,0(a0)#加载程序头地址
    li a2,0xffff0000
    add a0,a2,a0#计算物理起始地址，现在是减去0x100000的相对地址
    lui a2,0x80000#启动mmu
    add a1,a2,a1
    csrrw zero,satp,a1//将虚拟基址写入satp
    jalr zero,0(a0)#跳转到程序执行入口_start
    # ret