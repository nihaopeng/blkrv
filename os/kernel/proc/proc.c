#include "proc.h"

pcb global_pcb_list[MAX_PRO_NUM];

int init_ps(){
    for(int i=1;i<MAX_PRO_NUM;i++){
        global_pcb_list[i].is_alive=0;
    }
    //kernel process
    global_pcb_list[1].is_alive=1;
    __asm__ volatile(
        "csrr %0,satp\n"
        :
        :"r"(global_pcb_list[1].satp)//获取satp的值，也就是页表基址
    );
    global_pcb_list[1].stdout=-1;
}

//在mmu不使能状态下执行，此时satp为0，因此系统调用应使用kernel函数
int exec_i(uint32_t inode_id,int stdout,char** para,uint32_t para_num){
    uint8_t new_pid=0;
    //获取空闲pid
    //以下初始化pcb
    for(int i=1;i<MAX_PRO_NUM;i++){
        if(global_pcb_list[i].is_alive==0){
            new_pid=i;
            break;
        }
    }
    global_pcb_list[new_pid].is_alive = 1;
    global_pcb_list[new_pid].stdout=stdout;
    global_pcb_list[new_pid].satp=((uint32_t)new_pid)<<20 | alloc_page();

    //以下初始化栈空间
    uint32_t* stack_bottom=(uint32_t*)mallock(0x00100000,satp&0x000fffff);//1MB栈空间
    _vir2phy(uint32_t*,stack_bottom);

    //以下加载程序代码
    inode* ino;
    get_inode_by_id(inode_id,&ino);
    uint8_t* program_start=(uint8_t*)mallock(ino->size,satp&0x000fffff);//程序存放位置

    _vir2phy(uint8_t*,program_start);//转换为物理地址；
    
    uint8_t* program_cursor=program_start;
    char read_buf[512];
    uint32_t file_size=ino->size;
    while(file_size){
        if(file_size>=512){
            readk(inode_id,read_buf,0,512);
            for(int i=0;i<512;i++){
                *(program_cursor++)=read_buf[i];
            }
        }
        else{
            readk(inode_id,read_buf,0,file_size);
            for(int i=0;i<file_size;i++){
                *(program_cursor++)=read_buf[i];
            }
        }
    }

    //以下加载传入参数

    //更新栈底

    //更改satp使能mmu。

    //跳转到程序_start
    
}

int exit_i(){
    
}

int scheduler(){

}

_regist_syscall(void,exec);

_regist_syscall(void,exit);
