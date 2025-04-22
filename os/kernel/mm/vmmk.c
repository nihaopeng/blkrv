#include "mm.h"

uint8_t pages[MAX_PAGE]={0};

uint32_t alloc_page(){
    for(uint32_t i=(USER_START>>12);i<MAX_PAGE;i++){//从用户空间地址开始
        if(pages[i]==0){//页未被使用
            pages[i]=1;
            return i;//ppn
        }
    }
}

uint32_t free_page(uint32_t page){
    pages[page]=0;
}

//在页表中插入页表项
//TODO:虚拟地址分配也需要连续！！！
uint32_t insert_pte(uint32_t satp_ppn,uint32_t new_ppn){//TODO:融合到malloc进行性能优化
    
}

void init_vmm(){
    //前8MB存放内核程序提前占用。
    for(uint32_t i=0;i<(USER_START>>12);i++){
        pages[i]=1;
    }
    //构造线性内核空间，4MB
    //!!!从页表的第256页开始，也即0x00100000,bios的大小

    //TODO:可优化，不必要
    uint32_t page_contents_ppn=alloc_page();
    uint32_t page_list1_ppn=alloc_page();
    uint32_t page_list2_ppn=alloc_page();

    //构造页表目录
    uint32_t* page_contents_addr=(uint32_t*)((page_contents_ppn<<12)+RAM_START);//地址赋值的地方需要使用物理地址。
    uint32_t* page_list1_addr   =(uint32_t*)((page_list1_ppn   <<12)+RAM_START);
    uint32_t* page_list2_addr   =(uint32_t*)((page_list2_ppn   <<12)+RAM_START);
    *page_contents_addr    =(uint32_t)page_list1_addr | 0x00000001;//第一位为是否占用。
    *(page_contents_addr+1)=(uint32_t)page_list2_addr | 0x00000001;

    //构造页表，线性，也就是虚拟地址=物理地址
    uint32_t phy_addr=256;
    for(uint32_t i=256;i<1024;i++){//第一页从256项开始，1MB起始
        *(page_list1_addr+i)=((phy_addr<<12)) | 0x00000001;
        phy_addr+=1;
    }
    for(uint32_t i=0;i<768;i++){
        *(page_list2_addr+i)=((phy_addr<<12)) | 0x00000001;
        phy_addr+=1;
    }

    //启用mmu
    uint32_t satp=(uint32_t)page_contents_addr|1;//1号进程,页表目录的物理页号
    printk("page_contents:%x,page_list1:%x,satp:%x\n",page_contents_addr,*page_contents_addr,satp);

    __asm__ volatile( 
        "csrw satp,%0\n"
        "nop\n"
        "nop\n"
        :
        :"r"(satp)
    );
}

/*
    运行在机器模式
    在链表中找到合适的块，并分配，并重新构造链表
*/
uint32_t check_vir_block_list(uint32_t size,uint32_t* free_vir_block_list_addr){
    while(free_vir_block_list_addr){
        if(free_vir_block_list_addr->size>=size){
            //重新构造链表
            
            return free_vir_block_list_addr->vir_addr;
        }
        free_vir_block_list_addr=free_vir_block_list_addr->next;
    }
    return 0;
}

int freek(void* pointer,uint32_t satp_ppn,uint32_t* free_vir_block_list_addr){//pointer为虚拟地址
    
}

void* mallock(uint32_t size,uint32_t satp_ppn,uint32_t* free_vir_block_list_addr){//size为字节数
    //遍历空闲链表，分配合适的块

}