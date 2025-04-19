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
    uint32_t* stack_bottom=(uint32_t*)mallock(0x00100000,global_pcb_list[new_pid].satp&0x000fffff);//1MB栈空间
    // _vir2phy(uint32_t*,stack_bottom,global_pcb_list[new_pid].satp);

    //以下加载程序代码
    inode* ino;
    get_inode_by_id(inode_id,&ino);
    char read_buf[512];
    //根据程序头加载可执行文件。
    //读取文件头
    readk(inode_id,read_buf,0,52);
    //TODO:验证二进制魔数

    //程序头表存储地址的偏移量
    uint32_t rec_addr_prog_head=0x28;
    uint32_t rec_size_prog_head=0x2a;
    uint32_t rec_num_prog_head=0x2c;
    uint32_t rec_addr_start=0x18;

    //根据偏移量读取程序头表
    uint32_t prog_head_table_addr=*(uint32_t*)(read_buf+rec_addr_prog_head);
    uint32_t prog_head_size=*(uint32_t*)(read_buf+rec_size_prog_head);
    uint32_t prog_head_num=*(uint32_t*)(read_buf+rec_num_prog_head);
    uint32_t prog_start_addr=*(uint32_t*)(read_buf+rec_addr_start);

    readk(inode_id,read_buf,prog_head_table_addr,prog_head_size * prog_head_num);
    for(uint32_t i=0;i<prog_head_num;i++){
        uint32_t* prog_head_addr=(uint32_t*)(read_buf+i*prog_head_size);
        uint32_t prog_head_type=*(prog_head_addr+0);
        uint32_t prog_head_offset=*(prog_head_addr+1);
        uint32_t prog_head_vaddr=*(prog_head_addr+2);
        uint32_t prog_head_paddr=*(prog_head_addr+3);
        uint32_t prog_head_filesize=*(prog_head_addr+4);
        uint32_t prog_head_memsize=*(prog_head_addr+5);
        uint32_t prog_head_flags=*(prog_head_addr+6);
        uint32_t prog_head_align=*(prog_head_addr+7);
        if(prog_head_type==1){
            //加载可执行文件
            uint8_t* program_start=(uint8_t*)mallock(prog_head_memsize,global_pcb_list[new_pid].satp&0x000fffff);//程序存放位置
            _vir2phy(uint8_t*,program_start,global_pcb_list[new_pid].satp);//转换为物理地址；根据新satp转换，
            while(prog_head_filesize){
                if(prog_head_filesize>=512){
                    readk(inode_id,read_buf,prog_head_offset,512);
                    for(int i=0;i<512;i++){
                        *(program_start++)=read_buf[i];
                    }
                    prog_head_offset+=512;
                    prog_head_filesize-=512;
                }
                else{
                    readk(inode_id,read_buf,prog_head_offset,prog_head_filesize);
                    for(int i=0;i<prog_head_filesize;i++){
                        *(program_start++)=read_buf[i];
                    }
                    prog_head_offset+=prog_head_filesize;
                    prog_head_filesize=0;
                }
            }
        }
    }
    uint32_t argv=0;
    uint32_t argc=para_num;
    //以下加载传入参数
    for(int i=0;i<para_num;i++){
        uint32_t para_size=str_len(para[i])+1;
        uint8_t* para_addr=(uint8_t*)mallock(para_size,global_pcb_list[new_pid].satp&0x000fffff);
        argv=(uint32_t)para_addr;
        _vir2phy(uint8_t*,para_addr,global_pcb_list[new_pid].satp);
        for(int j=0;j<para_size;j++){
            *(para_addr++)=para[i][j];
        }
    }
    __asm__ volatile(
        "mv a0,%0\n"//argc
        "mv a1,%1\n"//argv
        "mv t0,%2\n"//栈底
        "mv sp,t0\n"//设置栈底
        "mv t0,%3\n"//设置栈顶
        "csrw satp,t0\n"
        "mv t0,%4\n"//设置satp
        "jalr ra,t0\n"
        :
        :"r"(argc),"r"(argv),"r"(stack_bottom),"r"(global_pcb_list[new_pid].satp),"r"(prog_start_addr)//设置satp
    );

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
