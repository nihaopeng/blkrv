#include "dt.h"

desc_table syscall_table;
desc_table inter_table;

void init_std(){
    int* gdt_addr_vprint=(int*)(&syscall_table[_NR_vprint]);
    int* gdt_addr_vgetch=(int*)(&syscall_table[_NR_vgetch]);
    int* idt_addr_kbdown=(int*)(&inter_table[_NI_kbdown]);
    int* gdt_addr_powoff=(int*)(&syscall_table[_NR_powoff]);
    int* gdt_addr_kbhit=(int*)(&syscall_table[_NR_kbhit]);
    regist_vprint(gdt_addr_vprint);//之所以在这里传参是为了避免两张表的地址被编译在GOT表中
    // regist_stdin(gdt_addr_vgetch);
    regist_keydown_int(idt_addr_kbdown);
    regist_poweroff(gdt_addr_powoff);
    regist_vgetch(gdt_addr_vgetch);
    regist_kbhit(gdt_addr_kbhit);
}