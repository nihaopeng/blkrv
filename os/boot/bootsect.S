flash_addr=0x40200
ram_addr=0x00100

flash_kernel_start=0x200
ram_kernel_start=0x200
#below is offset
rec_addr_prog_head=0x28
rec_size_prog_head=0x2a
rec_num_prog_head=0x2c
rec_addr_start=0x18

_start:
    addi x1,x0,0
    addi x2,x0,0
    lui x1,flash_addr#加载flash的首地址
    addi x1,x1,flash_kernel_start#定位到flash512字节
    lui x2,ram_addr#加载ram的首地址
    # addi x2,x2,ram_kernel_start#定位到ram512字节
    addi x3,x1,rec_addr_prog_head#记录程序头地址的地址
    lb x3,0(x3)#加载程序头地址
    add x3,x1,x3
    addi x4,x1,rec_size_prog_head#记录程序头大小的地址
    lb x4,0(x4)
    addi x5,x1,rec_num_prog_head#记录程序头数量的地址
    lb x5,0(x5)
.l1:
    addi x5,x5,-1#程序头数量减一
    addi x6,x3,4#程序头偏移四拿到记录在flash中偏移地址的地址
    lw x6,0(x6)
    addi x11,x3,12#程序头偏移四拿到记录在flash中偏移地址的地址
    lw x11,0(x11)#拿到物理地址
    addi x7,x3,16#拿到文件大小地址
    lw x7,0(x7)
    add x3,x3,x4
    add x8,x1,x6#flash段起始地址
    add x9,x2,x11#ram起始地址
    addi x11,zero,4
.l2:
    addi x7,x7,-4#需要加载的大小减4
    lw x10,0(x8)
    sw x10,0(x9)
    addi x8,x8,4
    addi x9,x9,4
    bgez x7,.l2#以上加载段数据,大于等于0跳转
    bnez x5,.l1
    addi x3,zero,0
    addi x4,zero,0
    addi x5,zero,0
    addi x6,zero,0
    addi x7,zero,0
    addi x8,zero,0
    addi x9,zero,0
    addi x10,zero,0
    addi x1,x1,rec_addr_start#记录程序entry地址的地址
    lw x1,0(x1)#加载entry地址
    add x1,x2,x1
    li sp,0x00400000//设置栈指针,ram起始地址偏移3MB,前2MB放内核程序
    li s0,0x00400000
    jalr zero,0(x1)






    
