#include "proc.h"

uint8_t pro_ids[MAX_PID_NUM]={0};
uint8_t virtual_addr_block[MAX_PID_NUM]={0};
pcb global_pcb_list[64];

int init_ps(){
    global_pcb_list[0].pid=0;
    global_pcb_list[0].virtual_base_addr=0x10000000;
    global_pcb_list[0].stdout=0;
    global_pcb_list[0].stdout_start=0;
}

int exec(uint32_t inode_id,int priority,int stdout,int stdout_start,int* pid,int* status,char* para[]){
    inode* ino;
    get_inode_by_id(inode_id,&ino);
    uint32_t i;
    for(i=1;i<MAX_PID_NUM;i++){
        if(!pro_ids[i]){
            global_pcb_list[i].pid=i;
            global_pcb_list[i].virtual_base_addr=0x10000000+(i<<23);
            global_pcb_list[i].heap_addr=0x10000000+(i<<23)+4*1024*1024;
            pro_ids[i]=1;
            goto flag;
        }
    }
    printk("num of proc reaches limitaion\n");
    *status=-1;
    return -1;
flag:
    // global_pcb_list[i].priority=priority;
    global_pcb_list[i].stdout=stdout;
    global_pcb_list[i].stdout_start=stdout_start;
    //load prog
    char buf[1024];
    memset_s(buf,0,1024);
    int j=0;
    printk("load prog...");
    char* st=(char*)global_pcb_list[i].virtual_base_addr;
    while(read_i(inode_id,buf,j,1024)!=-1){
        for(int k=0;k<1024;k++){
            *st=buf[k];
            st+=1;
        }
        j+=1024;
    }
    printk("start pcb...\n");
    start_pcb(i);
    *status=0;
}

int start_pcb(int pid){
    set_stdout(global_pcb_list[pid].stdout,global_pcb_list[pid].stdout_start);
    printk("enter_prog\n");
    enter_prog(global_pcb_list[pid].virtual_base_addr);
}