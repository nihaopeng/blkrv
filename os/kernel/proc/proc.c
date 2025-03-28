#include "proc.h"

uint8_t pro_ids[MAX_PID_NUM]={0};
pcb global_pcb_list[64];

int init_ps(){
    
}

int exec(uint32_t inode_id,int priority,int stdout,int stdout_start,int* pid,int* status,char** para,uint32_t para_num){
    
}

int exit_i(){
    
}


void regist_exit(int* gdt_addr_exit){
    int* func_addr_exit=(int*)(&exit_i);
    _set_gate(gdt_addr_exit,func_addr_exit);
}