#include "file.h"

int read_i(uint32_t inode_id,char* buf,uint32_t start,uint32_t count){//we hope your buf has been init;
    uint32_t p=0;
    __asm__ volatile(
        "csrr %0,0x181"
        :"=r"(p)
    );
    p=(p<<1)>>1;//去除mmu标志位
    // printk("%d\n",p);
    buf=(char*)((void*)buf+p);

    readk(inode_id,buf,start,count);
}

int write_i(uint32_t inode_id,char* buf,uint32_t start,uint32_t count){//the start should not bigger than size
    uint32_t p=0;
    __asm__ volatile(
        "csrr %0,0x181"
        :"=r"(p)
    );
    p=(p<<1)>>1;//去除mmu标志位
    buf=(char*)((void*)buf+p);

    writek(inode_id,buf,start,count);
}

int open_i(char* file_path,uint32_t* inode_id,int* status){
    uint32_t p=0;
    __asm__ volatile(
        "csrr %0,0x181"
        :"=r"(p)
    );
    p=(p<<1)>>1;//去除mmu标志位
    file_path=(char*)((void*)file_path+p);
    inode_id=(uint32_t*)((void*)inode_id+p);
    status=(int*)((void*)status+p);

    openk(file_path,inode_id,status);
}

int create_i(char* file_path,char type,uint32_t* inode_id,int* status){
    uint32_t p=0;
    __asm__ volatile(
        "csrr %0,0x181"
        :"=r"(p)
    );
    p=(p<<1)>>1;//去除mmu标志位
    file_path=(char*)((void*)file_path+p);
    inode_id=(uint32_t*)((void*)inode_id+p);
    status=(int*)((void*)status+p);

    createk(file_path,type,inode_id,status);
}

void regist_read(int* dt_adrr){
    int* func_addr_read=(int*)(&read_i);
    _set_gate(dt_adrr,func_addr_read);
}

void regist_write(int* dt_adrr){
    int* func_addr_write=(int*)(&write_i);
    _set_gate(dt_adrr,func_addr_write);
}

void regist_open(int* dt_adrr){
    int* func_addr_open=(int*)(&open_i);
    _set_gate(dt_adrr,func_addr_open);
}

void regist_create(int* dt_adrr){
    int* func_addr_create=(int*)(&create_i);
    _set_gate(dt_adrr,func_addr_create);
}
