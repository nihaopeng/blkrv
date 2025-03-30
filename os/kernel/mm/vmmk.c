#include "mm.h"

uint8_t pages[MAX_PAGE]={0};

int insert_pte(uint32_t* addr){

}

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

void init_vmm(){
    //前8MB存访内核程序提前占用。
    for(uint32_t i=0;i<(USER_START>>12);i++){
        pages[i]=1;
    }
    //构造线性内核空间，8MB,!!!从页表的第256页开始，也即0x00100000

    //TODO:可优化，不必要
    uint32_t page_contents_ppn=alloc_page();
    uint32_t page_list1_ppn=alloc_page();
    uint32_t page_list2_ppn=alloc_page();
    uint32_t page_list3_ppn=alloc_page();

    //构造页表目录
    uint32_t* page_contents_addr=(uint32_t*)((page_contents_ppn<<12)+RAM_START);//地址赋值的地方需要使用物理地址。
    uint32_t* page_list1_addr=(uint32_t*)((page_list1_ppn<<12)+RAM_START);
    uint32_t* page_list2_addr=(uint32_t*)((page_list2_ppn<<12)+RAM_START);
    uint32_t* page_list3_addr=(uint32_t*)((page_list3_ppn<<12)+RAM_START);
    *page_contents_addr=(uint32_t)page_list1_addr;
    *(page_contents_addr+1)=(uint32_t)page_list2_addr;
    *(page_contents_addr+2)=(uint32_t)page_list2_addr;

    //构造页表，线性，也就是虚拟地址=物理地址
    uint32_t phy_addr=0;
    for(uint32_t i=256;i<1024;i++){//第一页从256项开始，1MB起始
        *(page_list1_addr+i)=(phy_addr<<12)+RAM_START;
        phy_addr+=1;
    }
    for(uint32_t i=0;i<1024;i++){
        *(page_list2_addr+i)=(phy_addr<<12)+RAM_START;
        phy_addr+=1;
    }
    for(uint32_t i=0;i<256;i++){
        *(page_list3_addr+i)=(phy_addr<<12)+RAM_START;
        phy_addr+1;
    }

    //启用mmu
    uint32_t satp=(1<<20)|(uint32_t)page_contents_addr>>12;//1号进程,页表目录的物理页号
    printk("page_contents:%d,page_list1:%d,satp:%d\n",page_contents_addr,*page_contents_addr,satp);

    __asm__ volatile( \
        "csrrw zero,satp,%0\n"\
        : \
        :"r"(satp) \
    );

    print("finish\n");
}

int freek(void* pointer){
    
}

void* mallock(uint32_t size){
    uint32_t actual_size=size+4;
    uint32_t alloc_page_num=(actual_size+PAGE_SIZE-1)>>12;//计算需要的页数
    for(uint32_t i=(USER_START>>12),j=0;i<MAX_PAGE&&j<alloc_page_num;i++){//从用户空间地址开始
        if(pages[i]==0){//页未被使用
            uint32_t* page_addr=(uint32_t*)(i<<12);
            
        }
    }
}