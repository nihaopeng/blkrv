#below is offset
rec_addr_prog_head=0x28
rec_size_prog_head=0x2a
rec_num_prog_head=0x2c
rec_addr_start=0x18

.text
.global enter_prog
.global save_contxt

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
    ret#不涉及程序调用，无需进行ra保存

enter_prog:
    mv a1,a0
    addi a0,a0,rec_addr_start#记录程序main地址的地址
    lw a0,0(a0)#加载程序头地址
    li a2,0xffff0000
    add a0,a2,a0#计算物理起始地址
    addi a2,zero,0
    # add a0,a1,a0
    lui a2,0x80000#启动mmu
    add a1,a2,a1#加载虚拟地址
    csrrw zero,satp,a1
    jalr zero,0(a0)#跳转到程序执行入口_start