#include "dt.h"

desc_table syscall_table,inter_table;

void init_std(){
    int* gdt_addr_vprint=(int*)(&syscall_table[_NR_vprint]);
    int* gdt_addr_vgetch=(int*)(&syscall_table[_NR_vgetch]);
    regist_stdout(gdt_addr_vprint);//之所以在这里传参是为了避免两张表的地址被编译在GOT表中
    regist_stdin(gdt_addr_vgetch);
}