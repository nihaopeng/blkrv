#include "proc.h"

pcb global_pcb_list[MAX_PRO_NUM];

int exitk(){
    //获取调用exit的程序的进程号；
    uint32_t satp=0;
    __asm__ volatile("csrrw %0,0x181,zero":"=r"(satp));
    uint32_t pid=satp&0x00000fff;
    printk("process: %d, mem space free list:\n",pid);
    show_free_node_list((uint32_t*)(satp&0xfffff000),&(global_pcb_list[pid].free_block_head));
    printk("process exit wait to implement\n");
    shutdown();
    //回收程序内存空间。
    //调用schedular进入下一个进程。
}

int get_free_pid(){
    for(uint32_t i=1;i<MAX_PRO_NUM;i++){
        if(global_pcb_list[i].is_alive==0){
            return i;
        }
    }
}

uint32_t init_pcb(int new_pid,int stdout){
    global_pcb_list[new_pid].is_alive = 1;
    global_pcb_list[new_pid].stdout=stdout;
    global_pcb_list[new_pid].free_block_head.next=(mnode*)0x4;
    global_pcb_list[new_pid].free_block_head.size=0x0;
    uint32_t page_content_addr=(alloc_page()<<12) + RAM_START;
    //分配一页用于初始化
    uint32_t new_page=(alloc_page()<<12)+RAM_START;
    write_page_table((uint32_t*)page_content_addr,0,new_page,1);
    printk("page_content_addr:%x\n",page_content_addr);
    global_pcb_list[new_pid].satp=page_content_addr|new_pid;
    //初始化空闲列表头。
    mnode* node_in_page=(mnode*)vir2phy((uint32_t*)page_content_addr,0x4);
    printk("node_in_page:%x\n",node_in_page);
    node_in_page->next=0;
    node_in_page->size=0xffffff4;
    printk("global_pcb_list[%d].satp:%x\n",new_pid,global_pcb_list[new_pid].satp);
    return page_content_addr;
}

uint32_t load_program(uint32_t inode_id,uint32_t page_content_addr,mnode* free_block_head){
    //以下加载程序代码
    inode* ino;
    finfo_k(inode_id,ino);
    printk("file size:%d\n",ino->size);
    char read_buf[512];
    memset_s(read_buf,0,512);
    //根据程序头加载可执行文件。
    //读取文件头
    readk(inode_id,read_buf,0,52);
    printk("magic number:%c,%c,%c\n",read_buf[1],read_buf[2],read_buf[3]);
    //TODO:验证二进制魔数

    //程序头表存储地址的偏移量
    uint32_t rec_addr_prog_head=0x28;
    uint32_t rec_size_prog_head=0x2a;
    uint32_t rec_num_prog_head=0x2c;
    uint32_t rec_addr_start=0x18;

    //根据偏移量读取程序头表
    uint8_t prog_head_table_addr=*(read_buf+rec_addr_prog_head);
    uint8_t prog_head_size=*(read_buf+rec_size_prog_head);
    uint8_t prog_head_num=*(read_buf+rec_num_prog_head);
    uint32_t prog_start_addr=*(uint32_t*)(read_buf+rec_addr_start);
    printk("prog_head_table_addr:%d\n",prog_head_table_addr);
    printk("prog_head_size:%d\n",prog_head_size);
    printk("prog_head_num:%d\n",prog_head_num);
    printk("prog_start_addr:%d\n",prog_start_addr);

    readk(inode_id,read_buf,prog_head_table_addr,prog_head_size*prog_head_num);
    //计算最大malloc边界，以确保数据加载到正确的虚拟地址位置。
    uint32_t max_vir=0;
    for(uint32_t i=0;i<prog_head_num;i++){
        uint32_t* prog_head_addr=(uint32_t*)(read_buf+i*prog_head_size);
        uint32_t prog_head_type=*(prog_head_addr+0);
        uint32_t prog_head_vaddr=*(prog_head_addr+2);
        uint32_t prog_head_memsize=*(prog_head_addr+5);
        if(prog_head_type==1){
            max_vir=prog_head_vaddr+prog_head_memsize;
        }
    }
    uint32_t* program_malloc_space=(uint32_t*)mallock(max_vir,(uint32_t*)page_content_addr,free_block_head);
    char file_data_buf[512];
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
            // printk("prog_head_memsize:%x\n",prog_head_memsize);
            uint8_t* program_start_vir=(uint8_t*)prog_head_vaddr;//程序存放位置
            printk("program_start vir:%x\n",program_start_vir);
            uint8_t* program_start=(uint8_t*)vir2phy((uint32_t*)page_content_addr,(uint32_t)program_start_vir);//转换为物理地址；根据新satp转换，
            // printk("program_start:%x\n",program_start);
            readk(inode_id,file_data_buf,prog_head_offset,512);
            while(prog_head_filesize){
                debugk(prog_head_filesize);
                if(prog_head_filesize>=512){
                    readk(inode_id,file_data_buf,prog_head_offset,512);
                    for(int i=0;i<512;i++){
                        program_start_vir++;
                        *(program_start++)=file_data_buf[i];
                    }
                    prog_head_offset+=512;
                    prog_head_filesize-=512;
                    //达到新的一页，读取新的物理地址。
                    if((uint32_t)program_start_vir%PAGE_SIZE==0){
                        program_start=(uint8_t*)vir2phy((uint32_t*)page_content_addr,(uint32_t)program_start_vir);
                    }
                }
                else{
                    readk(inode_id,file_data_buf,prog_head_offset,prog_head_filesize);
                    for(int i=0;i<prog_head_filesize;i++){
                        *(program_start++)=file_data_buf[i];
                    }
                    prog_head_offset+=prog_head_filesize;
                    prog_head_filesize=0;
                }
            }
        }
    }
    return prog_start_addr;
}

uint32_t load_params(uint32_t para_num,char** para,uint32_t page_content_addr,mnode* free_block_head){
    uint32_t argc=para_num;
    //以下加载传入参数
    uint32_t argv=(uint32_t)mallock(sizeof(char*)*para_num,(uint32_t*)page_content_addr,free_block_head);//char* []的空间
    uint32_t* argv_phy=(uint32_t*)vir2phy((uint32_t*)page_content_addr,argv);
    for(int i=0;i<para_num;i++){
        printk("para:%d,%s\n",i,para[i]);
        show_free_node_list((uint32_t*)page_content_addr,free_block_head);
        uint32_t para_size=str_len(para[i])+1;
        uint8_t* para_addr=(uint8_t*)mallock(para_size,(uint32_t*)page_content_addr,free_block_head);
        // show_free_node_list((uint32_t*)page_content_addr,free_block_head);
        *(argv_phy++)=(uint32_t)para_addr;
        if((argv+i)%PAGE_SIZE==0)
            argv_phy=(uint32_t*)vir2phy((uint32_t*)page_content_addr,argv+i);
        para_addr=(uint8_t*)vir2phy((uint32_t*)page_content_addr,(uint32_t)para_addr);
        for(int j=0;j<para_size;j++){
            *(para_addr++)=para[i][j];
        }
        *para_addr='\0';
    }
    return argv;
}

uint32_t vmm_test(uint32_t page_content_addr,mnode* free_block_head){
    char* tmp=(char*)mallock(30,(uint32_t*)page_content_addr,free_block_head);
    printk("tmp:%x\n",tmp);
    char* tmp1=(char*)mallock(30,(uint32_t*)page_content_addr,free_block_head);
    printk("tmp1:%x\n",tmp1);
    char* tmp2=(char*)mallock(30,(uint32_t*)page_content_addr,free_block_head);
    printk("tmp2:%x\nfree tmp1",tmp2);
    freek(tmp1,(uint32_t*)page_content_addr,free_block_head);
}

int execk(uint32_t inode_id,int stdout,char** para,uint32_t para_num){
    uint32_t new_pid=get_free_pid();
    //获取空闲pid
    //以下初始化pcb
    uint32_t page_content_addr=init_pcb(new_pid,stdout);

    mnode* free_block_head=&(global_pcb_list[new_pid].free_block_head);
    
    uint32_t prog_start_addr=load_program(inode_id,page_content_addr,free_block_head);

    // vmm_test(page_content_addr,free_block_head);
    //以下初始化栈空间
    uint32_t* stack_top=(uint32_t*)mallock(0x100000,(uint32_t*)page_content_addr,free_block_head);//1MB栈空间
    // debugk(stack_top);
    uint32_t* stack_bottom=(uint32_t*)((void*)stack_top+0xffffc);
    // debugk(stack_bottom);
    uint32_t argv=load_params(para_num,para,page_content_addr,free_block_head);

    // vmm_test(page_content_addr,free_block_head);

    __asm__ volatile(
        "mv t0,%3\n"//加载程序页表基址
        "mv t1,%4\n"//加载程序入口地址
        "mv a0,%0\n"//argc
        "mv a1,%1\n"//argv
        "mv sp,%2\n"//栈底
        "csrw satp,t0\n"
        "jalr ra,t1\n"
        "nop\n"
        "nop\n"
        :
        :"r"(para_num),"r"(argv),"r"(stack_bottom),"r"(global_pcb_list[new_pid].satp),"r"(prog_start_addr)//设置satp
    );

    //更新栈底
    //更改satp使能mmu。
    //跳转到程序_start
}