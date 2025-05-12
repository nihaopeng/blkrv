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
    regist_mm();
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
    int* gdt_addr_finfo=(int*)(&syscall_table[_NR_finfo]);
    int* gdt_addr_delete=(int*)(&syscall_table[_NR_delete]);
    regist_read(gdt_addr_read);//之所以在这里传参是为了避免两张表的地址被编译在GOT表中
    regist_write(gdt_addr_write);
    regist_open(gdt_addr_open);
    regist_create(gdt_addr_create);
    regist_finfo(gdt_addr_finfo);
    regist_delete(gdt_addr_delete);
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
    int* gdt_addr_accept=(int*)(&syscall_table[_NR_accept]);
    int* gdt_addr_connect=(int*)(&syscall_table[_NR_connect]);
    int* gdt_addr_close=(int*)(&syscall_table[_NR_close]);
    regist_accept(gdt_addr_accept);//之所以在这里传参是为了避免两张表的地址被编译在GOT表中
    regist_connect(gdt_addr_connect);
    regist_send(gdt_addr_send);
    regist_recv(gdt_addr_recv);
    regist_close(gdt_addr_close);
}

void regist_graphic(){
    int* gdt_addr_draw_label=(int*)(&syscall_table[_NR_draw_label]);
    int* gdt_addr_draw_triangle=(int*)(&syscall_table[_NR_draw_triangle]);
    int* gdt_addr_flush=(int*)(&syscall_table[_NR_flush]);
    int* gdt_addr_draw_jpg=(int*)(&syscall_table[_NR_draw_jpg]);
    int* gdt_addr_draw_png=(int*)(&syscall_table[_NR_draw_png]);
    regist_draw_label(gdt_addr_draw_label);//之所以在这里传参是为了避免两张表的地址被编译在GOT表中
    regist_draw_triangle(gdt_addr_draw_triangle);
    regist_flush(gdt_addr_flush);
    regist_draw_jpg(gdt_addr_draw_jpg);
    regist_draw_png(gdt_addr_draw_png);
}

void regist_monitor(){
    int* gdt_addr_open_monitor=(int*)(&syscall_table[_NR_open_monitor]);
    int* gdt_addr_close_monitor=(int*)(&syscall_table[_NR_close_monitor]);
    regist_open_monitor(gdt_addr_open_monitor);//之所以在这里传参是为了避免两张表的地址被编译在GOT表中
    regist_close_monitor(gdt_addr_close_monitor);
}

void regist_mm(){
    int* gdt_addr_malloc=(int*)(&syscall_table[_NR_malloc]);
    int* gdt_addr_free=(int*)(&syscall_table[_NR_free]);
    regist_malloc(gdt_addr_malloc);//之所以在这里传参是为了避免两张表的地址被编译在GOT表中
    regist_free(gdt_addr_free);
}