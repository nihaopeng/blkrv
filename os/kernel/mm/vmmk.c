#include "proc.h"

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
    return (page_list_addr[page_list_offset]&0xfffff000)|(vir_addr&0x00000fff);
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

uint32_t show_free_node_list(uint32_t* page_content_addr,mnode* free_block_head){
    mnode* list_node_phy=free_block_head;
    mnode* list_node_vir=(mnode*)0xffffffff;
    do{
        if(list_node_vir==0){
            // printk("list_node_phy->size:%d\n",list_node_phy->size);
            break;
        }
        printk("(%x)&(%x)-> ",list_node_vir,list_node_phy->size);
        list_node_vir=list_node_phy->next;
        // printk("list_node_vir:%x\n",list_node_vir);
        list_node_phy=(mnode*)vir2phy(page_content_addr,(uint32_t)list_node_vir);
        // printk("list_node_phy:%x\n",list_node_phy);
    }while(list_node_phy);
    printk("\n");
}

//释放虚拟地址范围内的所有页,起始地址向下对齐到4KB，结束地址向上对齐到4KB
uint32_t free_page_table(uint32_t* page_content_addr,uint32_t vir_start,uint32_t vir_end){
    uint32_t free_block_start_addr=(vir_start+PAGE_SIZE-1)&0xfffff000;
    uint32_t free_block_end_addr=vir_end&0xfffff000;
    for(uint32_t i=free_block_start_addr;i<free_block_end_addr;i+=PAGE_SIZE){
        uint32_t page=vir2phy(page_content_addr,free_block_start_addr)>>12;
        free_page(page);
        write_page_table(page_content_addr,i,0,0);
    }
}

//将虚拟地址范围分配页,起始地址向上对齐到4KB，结束地址向下对齐到4KB
uint32_t alloc_page_table(uint32_t* page_content_addr,uint32_t vir_start,uint32_t vir_end){
    uint32_t block_start_addr=vir_start&0xfffff000;
    uint32_t block_end_addr=(vir_end+PAGE_SIZE-1)&0xfffff000;
    for(uint32_t i=block_start_addr;i<block_end_addr;i+=PAGE_SIZE){
        uint32_t phyaddr=(alloc_page()<<12)+RAM_START;
        write_page_table(page_content_addr,i,phyaddr,1);
    }
}

int freek(void* pointer,uint32_t* page_content_addr,mnode* free_block_head){//pointer为虚拟地址
    show_free_node_list(page_content_addr,free_block_head);
    uint32_t used_node_vir=(uint32_t)pointer-sizeof(mnode);
    mnode* used_node_phy=(mnode*)vir2phy(page_content_addr,used_node_vir);
    // printk("free size:%d\n",used_node_phy->size);
    if(!used_node_phy){
        return -1;
    }
    mnode* list_node_phy=free_block_head;
    uint32_t list_node_vir=0x0;
    mnode* prev_node_phy=list_node_phy;
    uint32_t prev_node_vir=0x0;
    //定位前后空闲节点
    do{
        //当前空闲块节点位于used_node_phy的后面
        if(list_node_vir>=used_node_vir){
            // printk("list_node_vir:%x\n",list_node_vir);
            break;
        }
        //到下一个节点
        prev_node_phy=list_node_phy;
        prev_node_vir=list_node_vir;
        list_node_vir=(uint32_t)(list_node_phy->next);
        list_node_phy=(mnode*)vir2phy(page_content_addr,list_node_vir);
    }while(list_node_phy);
    uint32_t free_vir_start=0;
    uint32_t free_vir_end=0;
    //向前合并
    debugk(prev_node_vir+sizeof(mnode)+prev_node_phy->size);
    debugk(used_node_vir);
    if(prev_node_vir+sizeof(mnode)+prev_node_phy->size==used_node_vir){//可以合并
        free_vir_start=prev_node_vir+sizeof(mnode);
        //对齐4096KB，如果在free返回则重新减小边界，将页free限制在used_node的范围，同时消除物理页碎片
        free_vir_start=(used_node_vir&0xfffff000)>free_vir_start?(used_node_vir&0xfffff000):free_vir_start;
        prev_node_phy->size+=sizeof(mnode)+used_node_phy->size;
        //向后合并
        if(used_node_vir+sizeof(mnode)+used_node_phy->size==list_node_vir){//可以合并
            debugk(list_node_vir);
            debugk(list_node_phy->size);
            free_vir_end=list_node_vir+sizeof(mnode)+list_node_phy->size;
            //对齐4096KB，如果在free返回则重新减小边界，将页free限制在used_node的范围，同时消除物理页碎片
            free_vir_end=((used_node_vir+PAGE_SIZE-1)&0xfffff000)<free_vir_end?((used_node_vir+PAGE_SIZE-1)&0xfffff000):free_vir_end;
            prev_node_phy->size+=sizeof(mnode)+list_node_phy->size;
            prev_node_phy->next=list_node_phy->next;
        }else{//不可合并
            free_vir_end=used_node_vir+sizeof(mnode)+used_node_phy->size;
            prev_node_phy->next=(mnode*)list_node_vir;
        }
    }else{//不可合并
        free_vir_start=used_node_vir;
        prev_node_phy->next=(mnode*)used_node_vir;
        //向后合并
        debugk(used_node_vir+sizeof(mnode)+used_node_phy->size);
        debugk(list_node_vir);
        if(used_node_vir+sizeof(mnode)+used_node_phy->size==list_node_vir){//可以合并
            free_vir_end=list_node_vir+sizeof(mnode)+list_node_phy->size;
            free_vir_end=((used_node_vir+PAGE_SIZE-1)&0xfffff000)<free_vir_end?((used_node_vir+PAGE_SIZE-1)&0xfffff000):free_vir_end;
            used_node_phy->size=used_node_phy->size+sizeof(mnode)+list_node_phy->size;
            used_node_phy->next=list_node_phy->next;
        }else{//不可合并
            free_vir_end=used_node_vir+sizeof(mnode)+used_node_phy->size;
            used_node_phy->next=(mnode*)list_node_vir;
        }
    }
    debugk(free_vir_start);
    debugk(free_vir_end);
    free_page_table(page_content_addr,free_vir_start,free_vir_end);
    show_free_node_list(page_content_addr,free_block_head);
    return 0;
}

void* mallock(uint32_t size,uint32_t* page_content_addr,mnode* free_block_head){//size为字节数
    //遍历空闲链表，分配合适的块
    // printk("page_content_addr:%x,free_block_head:%x\nstart malloc...\n");
    show_free_node_list(page_content_addr,free_block_head);
    mnode* list_node_phy=free_block_head;
    uint32_t list_node_vir=0x0;
    mnode* prev_node_phy=list_node_phy;
    do{
        // printk("cur_node_size:%d\n",list_node_phy->size);
        if(list_node_phy->size >= size){    
            //分配物理页
            uint32_t vir_start=list_node_vir+sizeof(mnode);
            uint32_t vir_end=list_node_phy->size!=size?list_node_vir+sizeof(mnode)*2+size:list_node_vir+sizeof(mnode)+size;//是否产生新的空闲碎片
            alloc_page_table(page_content_addr,vir_start,vir_end);
            //重新构造链表
            if(list_node_phy->size!=size){//如果产生新的空闲块//TODO:fix bug
                uint32_t new_free_node_vir=list_node_vir+sizeof(mnode)+size;
                mnode* new_free_node_phy=(mnode*)vir2phy(page_content_addr,new_free_node_vir);
                // printk("new_free_node_phy:%x,list_node_vir:%x,list_node_phy:%x\n",new_free_node_phy,list_node_vir,list_node_phy);
                // printk("list_node_phy->size:%x,size:%d,mnode:%d\n",list_node_phy->size,size,sizeof(mnode));
                new_free_node_phy->size=list_node_phy->size-size-sizeof(mnode);
                new_free_node_phy->next=list_node_phy->next;
                prev_node_phy->next=(mnode*)new_free_node_vir;
            }else{
                prev_node_phy->next=list_node_phy->next;
            }
            list_node_phy->size=size;
            list_node_phy->next=NULL;
            show_free_node_list(page_content_addr,free_block_head);
            return (void*)(list_node_vir+sizeof(mnode));
        }
        //到下一个节点
        prev_node_phy=list_node_phy;
        list_node_vir=(uint32_t)(list_node_phy->next);
        list_node_phy=(mnode*)vir2phy(page_content_addr,list_node_vir);
    }while(list_node_phy);
    return NULL;
}