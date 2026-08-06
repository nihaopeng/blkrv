#include "proc.h"

//在mmu不使能状态下执行，此时satp为0，因此系统调用应使用kernel函数
int exec_i(uint32_t inode_id,int stdout,char** para,uint32_t para_num){
    _vir2phyk(char**,para);
    for(int i=0;i<para_num;i++){
        _vir2phyk(char*,para[i]);
    }
    return execk(inode_id,stdout,para,para_num);
}

int exit_i(){
    exitk();
}

_regist_syscall(void,exec);
_regist_syscall(void,exit);
