#include "mm.h"

uint8_t pages[MAX_PAGE]={0};


//在页表中插入页表项
//TODO:虚拟地址分配也需要连续！！！
uint32_t insert_pte(uint32_t satp_ppn,uint32_t new_ppn){//TODO:融合到malloc进行性能优化
    uint32_t* page_content_addr=(uint32_t*)(satp_ppn<<12);
    for(uint32_t i=0;i<(PAGE_SIZE>>2);i++){
        uint32_t flags=(*(page_content_addr+i))&0x00000fff;//获取pte的标志位,第一位代表是否被占用
        if(flags)//为1则被占用，可以遍历二级页表
        {
            uint32_t* page_list_addr=(uint32_t*)((*(page_content_addr+i))&0xfffff000);//获取二级页表的基址
            for(uint32_t j=0;j<(PAGE_SIZE>>2);j++){//遍历二级页表
                uint32_t page_list_flags=(*(page_list_addr+j))&0x00000fff;
                if(!page_list_flags){//flags为0，二级页表项未被占用
                    *(page_list_addr+j)=(new_ppn<<12)|0x00000001;
                    return (i<<22)|(j<<12);//返回虚拟地址
                }
            }
        }else{//所有的二级页表被占用完了，需要新开一页
            uint32_t new_page_list_ppn=alloc_page();
            *(page_content_addr+i)=(new_page_list_ppn<<12)|0x00000001;
            *(uint32_t*)(new_page_list_ppn<<12)=(new_ppn<<12)|0x00000001;
            return (i<<22);//第二级页表是新的，所以中间10为直接为0即可。
        }
    }
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
    //构造线性内核空间，8MB,!!!从页表的第256页开始，也即0x00100000,bios的大小

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
    *page_contents_addr=((uint32_t)page_list1_addr) | 0x00000001;//第一位为是否占用。
    *(page_contents_addr+1)=(uint32_t)page_list2_addr | 0x00000001;
    *(page_contents_addr+2)=(uint32_t)page_list2_addr | 0x00000001;

    //构造页表，线性，也就是虚拟地址=物理地址
    uint32_t phy_addr=0;
    for(uint32_t i=256;i<1024;i++){//第一页从256项开始，1MB起始
        *(page_list1_addr+i)=((phy_addr<<12)+RAM_START) | 0x00000001;
        phy_addr+=1;
    }
    for(uint32_t i=0;i<1024;i++){
        *(page_list2_addr+i)=((phy_addr<<12)+RAM_START) | 0x00000001;
        phy_addr+=1;
    }
    for(uint32_t i=0;i<256;i++){
        *(page_list3_addr+i)=((phy_addr<<12)+RAM_START) | 0x00000001;
        phy_addr+1;
    }

    //启用mmu
    uint32_t satp=(1<<20)|(uint32_t)page_contents_addr>>12;//1号进程,页表目录的物理页号
    // printk("page_contents:%d,page_list1:%d,satp:%d\n",page_contents_addr,*page_contents_addr,satp);

    __asm__ volatile( \
        "csrrw zero,satp,%0\n"\
        : \
        :"r"(satp) \
    );

    // print("finish\n");
}

int freek(uint32_t satp_ppn,void* pointer){//pointer为虚拟地址
    uint32_t* page_content_addr=(uint32_t*)(satp_ppn<<12);
    uint32_t* page_list_addr=page_content_addr+((uint32_t)pointer&0xffc00000);
    uint32_t* page_addr=page_list_addr+((uint32_t)pointer&0x003ff000);
    uint32_t page_num=*page_addr;

    for(uint32_t i=(uint32_t)pointer&0xffc00000;i<(PAGE_SIZE>>2);i++){
        page_list_addr=(uint32_t*)((*(page_content_addr+i))&0xfffff000);//获取二级页表的基址
        for(uint32_t j=(uint32_t)pointer&0x003ff000;j<(PAGE_SIZE>>2);j++){//遍历二级页表
            uint32_t pte_page_id=((*(page_list_addr+j))>>12);
            free_page(pte_page_id);
            *(page_list_addr+j)=0;
            page_num-=1;
            if(page_num==0) return 0;
        }
        uint32_t page_list_page_id=(*(page_content_addr+i))>>12;
        free_page(page_list_page_id);
    }
}

void* mallock(uint32_t size,uint32_t satp_ppn){
    uint32_t actual_size=size+4;
    uint32_t alloc_page_num=(actual_size+PAGE_SIZE-1)>>12;//计算需要的页数
    uint32_t allocated_page_ppn=alloc_page();
    void* first_page_addr=(void*)insert_pte(satp_ppn,allocated_page_ppn);
    *(uint32_t*)(allocated_page_ppn<<12)=alloc_page_num;//赋值header为分配的页数。
    for(uint32_t j=1;j<alloc_page_num;j++){//从用户空间地址开始
        uint32_t allocated_page_ppn=alloc_page();
        insert_pte(satp_ppn,allocated_page_ppn);
    }
    return first_page_addr+4;//返回虚拟地址加4，前4字节用来存储指针header。
}