_jump:
    jal x0,_start

interrupt_table:
    .zero 1020

_start:
    addi x1,x0,0
    lui x1,0x60000#加载flash的首地址
    lui x2,0x10000#加载ram的首地址
    addi x2,x2,0x400#定位到ram0x800
    addi x2,x2,0x400#定位到ram0x800
    addi x3,x0,0x200
loop:
    lw x4,0(x1)#从flash加载数据到x4
    sw x4,0(x2)#将数据存到ram
    addi x1,x1,4#开始偏移地址
    addi x2,x2,4
    addi x3,x3,-4#计数器
    bnez x3,loop#不相等跳转
    lui x2,0x10000#加载ram的首地址
    addi x2,x2,0x400#定位到ram0x800
    addi x2,x2,0x400#定位到ram0x800
    jalr x0,0(x2)

    


