#include "proc.h"

extern pcb global_pcb_list[MAX_PRO_NUM];

int free_i(void* pointer){
    uint32_t satp=0;
    __asm__ volatile("csrr %0,0x181\n":"=r"(satp):);//获取satp的值，也就是页表基址
    /*
        以下操作是由于global_pcb_list是外部引用，无法获取其实际物理地址偏移
        只能获得相对位置，由于os kernel加载时是按照从ram起始地址0x10000，因此
        此处获得的虚拟地址也可以参考，只需要加上ram的物理起始地址即可。
    */
    mnode* free_block_head=(mnode*)((void*)&(global_pcb_list[satp&0x00000fff].free_block_head)+RAM_START);
    return freek(pointer,(uint32_t*)(satp&0xfffff000),free_block_head);
}

void* malloc_i(uint32_t size){
    uint32_t satp=0;
    __asm__ volatile("csrr %0,0x181":"=r"(satp):);//获取satp的值，也就是页表基址
    // printk("malloc satp:%x\n",satp);
    /*
        以下操作是由于global_pcb_list是外部引用，无法获取其实际物理地址偏移
        只能获得相对位置，由于os kernel加载时是按照从ram起始地址0x10000，因此
        此处获得的虚拟地址也可以参考，只需要加上ram的物理起始地址即可。
    */
    mnode* free_block_head=(mnode*)((void*)&(global_pcb_list[satp&0x00000fff].free_block_head)+RAM_START);
    return mallock(size,(uint32_t*)(satp&0xfffff000),free_block_head);
}

_regist_syscall(void,free);

_regist_syscall(void,malloc);

int memset_s(char* addr,char ch,uint32_t count){
    for(int i=0;i<count;i++){
        *(addr) = ch;
        addr+=1;
    }
    return 0;
}