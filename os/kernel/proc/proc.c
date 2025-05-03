#include "proc.h"

int init_ps(){
    // for(int i=1;i<MAX_PRO_NUM;i++){
    //     global_pcb_list[i].is_alive=0;
    // }
    // //kernel process
    // global_pcb_list[1].is_alive=1;
    // __asm__ volatile(
    //     "csrr %0,satp\n"
    //     :
    //     :"r"(global_pcb_list[1].satp)//获取satp的值，也就是页表基址
    // );
    // mnode* free_vir_block_list=(mnode*)global_pcb_list[1].satp;
    // free_vir_block_list->next=0;
    // free_vir_block_list->size=0xffffffff;
    // global_pcb_list[1].stdout=-1;
}

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

int scheduler(){

}

_regist_syscall(void,exec);

_regist_syscall(void,exit);
