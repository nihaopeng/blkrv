#include "dt.h"

desc_table syscall_table;
desc_table inter_table;

void regist_sysmethod(){
    regist_std();
    regist_ps();
    regist_fs();
    regist_net();
    regist_graphic();
    regist_monitor();
}

void regist_std(){
    int* gdt_addr_vprint=(int*)(&syscall_table[_NR_vprint]);
    int* gdt_addr_vgetch=(int*)(&syscall_table[_NR_vgetch]);
    int* idt_addr_kbdown=(int*)(&inter_table[_NI_kbdown]);
    int* gdt_addr_powoff=(int*)(&syscall_table[_NR_powoff]);
    int* gdt_addr_kbhit=(int*)(&syscall_table[_NR_kbhit]);
    regist_vprint(gdt_addr_vprint);//之所以在这里传参是为了避免两张表的地址被编译在GOT表中
    regist_keydown_interrupt(idt_addr_kbdown);
    regist_poweroff(gdt_addr_powoff);
    regist_vgetch(gdt_addr_vgetch);
    regist_kbhit(gdt_addr_kbhit);
}

void regist_fs(){
    int* gdt_addr_read=(int*)(&syscall_table[_NR_read]);
    int* gdt_addr_write=(int*)(&syscall_table[_NR_write]);
    int* gdt_addr_open=(int*)(&syscall_table[_NR_open]);
    int* gdt_addr_create=(int*)(&syscall_table[_NR_create]);
    regist_read(gdt_addr_read);//之所以在这里传参是为了避免两张表的地址被编译在GOT表中
    regist_write(gdt_addr_write);
    regist_open(gdt_addr_open);
    regist_create(gdt_addr_create);
}

void regist_ps(){
    int* gdt_addr_exit=(int*)(&syscall_table[_NR_exit]);
    int* gdt_addr_exec=(int*)(&syscall_table[_NR_exec]);
    regist_exec(gdt_addr_exec);
    regist_exit(gdt_addr_exit);//之所以在这里传参是为了避免两张表的地址被编译在GOT表中
}

void regist_net(){
    int* gdt_addr_send=(int*)(&syscall_table[_NR_send]);
    int* gdt_addr_recv=(int*)(&syscall_table[_NR_recv]);
    regist_send(gdt_addr_send);//之所以在这里传参是为了避免两张表的地址被编译在GOT表中
    regist_recv(gdt_addr_recv);
}

void regist_graphic(){
    int* gdt_addr_draw_label=(int*)(&syscall_table[_NR_draw_label]);
    int* gdt_addr_draw_triangle=(int*)(&syscall_table[_NR_draw_triangle]);
    int* gdt_addr_flush=(int*)(&syscall_table[_NR_flush]);
    regist_draw_label(gdt_addr_draw_label);//之所以在这里传参是为了避免两张表的地址被编译在GOT表中
    regist_draw_triangle(gdt_addr_draw_triangle);
    regist_flush(gdt_addr_flush);
}

void regist_monitor(){
    int* gdt_addr_open_monitor=(int*)(&syscall_table[_NR_open_monitor]);
    int* gdt_addr_close_monitor=(int*)(&syscall_table[_NR_close_monitor]);
    regist_open_monitor(gdt_addr_open_monitor);//之所以在这里传参是为了避免两张表的地址被编译在GOT表中
    regist_close_monitor(gdt_addr_close_monitor);
}