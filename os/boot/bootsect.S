
kernel_size=0x100#1MB
kernel_start=0x34

_start:
    addi x1,x0,0
    lui x1,0x60000#加载flash的首地址
    addi x1,x1,0x200#定位到flash512字节
    lui x2,0x10000#加载ram的首地址
    addi x2,x2,0x400#定位到ram0xa00
    addi x2,x2,0x600#定位到ram0xa00
    lui x3,kernel_size#内核大小,取整
loop:
    lw x4,0(x1)#从flash加载数据到x4
    sw x4,0(x2)#将数据存到ram
    addi x1,x1,4#计算偏移地址
    addi x2,x2,4
    addi x3,x3,-4#计数器
    bnez x3,loop#不相等跳转
    lui x2,0x10000#加载ram的首地址
    addi x2,x2,0x400
    addi x2,x2,0x600
    addi x2,x2,kernel_start
    jalr x0,0(x2)
