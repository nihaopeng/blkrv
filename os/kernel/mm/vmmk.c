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

/*
    写页表
    mode：1:分配，0:擦除
    如果页存在，则不进行写入
    如果页不存在，则分配一个页
*/
uint32_t write_page_table(uint32_t* page_content_addr,uint32_t vir_addr,uint32_t phy_addr,uint32_t mode){
    uint32_t pc_offset=vir_addr>>22;
    uint32_t page_list_offset=(vir_addr>>12)&0x000003ff;
    uint32_t* page_list_addr=(uint32_t*)(page_content_addr[pc_offset]&0xfffff000);
    if(mode==1&&page_list_addr==0){
        uint32_t page_list_ppn=alloc_page();
        if(page_list_ppn==0){
            return 0;
        }
        page_list_addr=(uint32_t*)((page_list_ppn<<12)+RAM_START);
        page_content_addr[pc_offset]=(((uint32_t)page_list_addr)|0x00000001);
    }
    if(mode==1&&page_list_addr[page_list_offset]==0){
        page_list_addr[page_list_offset]=phy_addr|0x00000001;
    }
    if(mode==0){
        page_list_addr[page_list_offset]=0;
    }
    return 1;
}

//返回虚拟地址对应的物理地址
uint32_t vir2phy(uint32_t* page_content_addr,uint32_t vir_addr){
    uint32_t pc_offset=vir_addr>>22;
    uint32_t page_list_offset=(vir_addr>>12)&0x000003ff;
    uint32_t* page_list_addr=(uint32_t*)(page_content_addr[pc_offset]&0xfffff000);
    if(page_list_addr==0){
        return 0;
    }
    return page_list_addr[page_list_offset]|vir_addr&0x00000fff;
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
    //构造页表目录
    uint32_t* page_contents_addr=(uint32_t*)((page_contents_ppn<<12)+RAM_START);//地址赋值的地方需要使用物理地址。
    //构造页表，线性，也就是虚拟地址=物理地址
    printk("init kernel page table\n");
    for(uint32_t i=256;i<256*5;i++){//一直到物理地址5MB，排除掉bios 1MB
        printk("page:%d\r",i);
        write_page_table(page_contents_addr,i<<12,i<<12,1);
    }

    //启用mmu
    uint32_t satp=(uint32_t)page_contents_addr|1;//1号进程,页表目录的物理页号
    printk("page_contents:%x,page_list1:%x,page1_addr:%x\n",page_contents_addr,*page_contents_addr,*(uint32_t*)(*page_contents_addr));

    __asm__ volatile( 
        "csrw satp,%0\n"
        "nop\n"
        "nop\n"
        :
        :"r"(satp)
    );
}

uint32_t show_free_node_list(uint32_t* page_content_addr){
    mnode* list_node_phy=(mnode*)page_content_addr;
    mnode* list_node_vir=0x0;
    do{
        if(list_node_phy->size==0){
            break;
        }
        printk("free node:%x, size:%d\n",list_node_vir,list_node_phy->size);
        list_node_vir=list_node_phy->next;
        list_node_phy=(mnode*)vir2phy(page_content_addr,(uint32_t)list_node_vir);
    }while(list_node_phy->next);
}

//释放虚拟地址范围内的所有页,起始地址向下对齐到4KB，结束地址向上对齐到4KB
uint32_t free_page_table(uint32_t* page_content_addr,uint32_t vir_start,uint32_t vir_end){
    uint32_t free_block_start_addr=(vir_start+PAGE_SIZE-1)&0xfffff000;
    uint32_t free_block_end_addr=vir_end&0xfffff000-PAGE_SIZE;
    for(uint32_t i=free_block_start_addr;i<free_block_end_addr;i+=PAGE_SIZE){
        uint32_t page=vir2phy(page_content_addr,free_block_start_addr)>>12;
        free_page(page);
        write_page_table(page_content_addr,i,0,0);
    }
}

int freek(void* pointer,uint32_t* page_content_addr){//pointer为虚拟地址
    show_free_node_list(page_content_addr);
    mnode* used_node_vir=(mnode*)((uint32_t)pointer-sizeof(mnode));
    mnode* used_node_phy=(mnode*)vir2phy(page_content_addr,(uint32_t)used_node_vir);
    if(!used_node_phy){
        return -1;
    }
    mnode* list_node_phy=(mnode*)page_content_addr;
    mnode* list_node_vir=0x0;
    mnode* prev_node_phy=list_node_phy;
    mnode* prev_node_vir=0x0;
    //定位前后空闲节点
    do{
        //当前空闲块节点位于used_node_phy的后面
        if(list_node_vir>=used_node_vir){
            break;
        }
        //到下一个节点
        prev_node_phy=list_node_phy;
        prev_node_vir=list_node_vir;
        list_node_vir=list_node_phy->next;
        list_node_phy=(mnode*)vir2phy(page_content_addr,(uint32_t)list_node_vir);
    }while(list_node_phy->next);
    uint32_t free_vir_start=0;
    uint32_t free_vir_end=0;
    //向前合并
    if(prev_node_vir+sizeof(mnode)+prev_node_phy->size==used_node_vir){
        free_vir_start=(uint32_t)prev_node_vir+sizeof(mnode);
        prev_node_phy->size+=sizeof(mnode)+used_node_phy->size;
        //向后合并
        if(used_node_vir+sizeof(mnode)+used_node_phy->size==list_node_vir){
            free_vir_end=(uint32_t)list_node_vir+sizeof(mnode)+list_node_phy->size;
            prev_node_phy->size+=sizeof(mnode)+used_node_phy->size;
            prev_node_phy->next=list_node_phy->next;
        }else{
            free_vir_end=(uint32_t)used_node_vir+sizeof(mnode)+used_node_phy->size;
        }
    }else{
        free_vir_start=(uint32_t)used_node_vir;
        prev_node_phy->next=used_node_phy;
        //向后合并
        if(used_node_vir+sizeof(mnode)+used_node_phy->size==list_node_vir){
            free_vir_end=(uint32_t)list_node_vir+sizeof(mnode)+list_node_phy->size;
            used_node_phy->size+=sizeof(mnode)+used_node_phy->size;
            used_node_phy->next=list_node_phy;
        }else{
            free_vir_end=(uint32_t)used_node_vir+sizeof(mnode)+used_node_phy->size;
        }
    }
    free_page_table(page_content_addr,free_vir_start,free_vir_end);
    show_free_node_list(page_content_addr);
    return 0;
}

void* mallock(uint32_t size,uint32_t* page_content_addr){//size为字节数
    //遍历空闲链表，分配合适的块
    mnode* head_node_phy=(mnode*)page_content_addr;
    mnode* list_node_phy=head_node_phy;
    mnode* list_node_vir=0x0;
    mnode* prev_node_phy=list_node_phy;
    mnode* prev_node_vir=0x0;
    do{
        if(list_node_phy->size >= size+sizeof(mnode)){
            uint32_t is_head=0;
            //初始化节点
            mnode* used_node_phy=NULL;
                
            if(list_node_vir!=0){
                used_node_phy=(mnode*)((uint32_t)list_node_phy);
            }else{
                is_head=1;
                used_node_phy=(mnode*)((uint32_t)list_node_phy+sizeof(mnode));
            }
            //分配物理页
            for(uint32_t i=0;i<(size+sizeof(mnode)+PAGE_SIZE-1)/PAGE_SIZE;i++){
                uint32_t page=alloc_page();
                if(page==0){
                    return NULL;
                }
                //写入页表
                uint32_t res=0;
                if(is_head==1)//头节点块分配需要保留头节点
                {
                    res=write_page_table(page_content_addr,(uint32_t)list_node_vir+sizeof(mnode)+i*PAGE_SIZE,(page<<12)+RAM_START,1);
                }
                else//非头节点分配直接将原节点作为used_node
                {
                    res=write_page_table(page_content_addr,(uint32_t)list_node_vir+i*PAGE_SIZE,(page<<12)+RAM_START,1);
                }
                if(!res){
                    return NULL;
                }
            }
            //重新构造链表
            uint32_t is_create_new_free_block=is_head?(list_node_phy->size==size?0:1):(list_node_phy->size==size+sizeof(mnode)?0:1);
            used_node_phy->size=size;
            used_node_phy->next=NULL;
            if(is_create_new_free_block){//如果产生新的空闲块
                mnode* used_node_vir=is_head?list_node_vir+sizeof(mnode):list_node_vir;
                mnode* used_node_phy=(mnode*)vir2phy(page_content_addr,(uint32_t)used_node_vir);
                used_node_phy->size=size-sizeof(mnode);
                used_node_phy->next=list_node_phy->next;
                prev_node_phy->next=used_node_phy;
            }
            return (void*)((uint32_t)list_node_vir+sizeof(mnode));
        }
        //到下一个节点
        prev_node_phy=list_node_phy;
        prev_node_vir=list_node_vir;
        list_node_vir=list_node_phy->next;
        list_node_phy=(mnode*)vir2phy(page_content_addr,(uint32_t)list_node_vir);
    }while(list_node_phy->next);
    show_free_node_list(page_content_addr);
    return NULL;
}