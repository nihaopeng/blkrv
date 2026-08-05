#include "proc.h"

pcb global_pcb_list[MAX_PRO_NUM];

int exitk(){
    //获取调用exit的程序的进程号；
    uint32_t satp=0;
    __asm__ volatile("csrr %0,0x181":"=r"(satp));
    uint32_t pid=satp&0x00000fff;
    printk("process: %d, mem space free list:\n",pid);
    show_free_node_list((uint32_t*)(satp&0xfffff000),&(global_pcb_list[pid].free_block_head));
    // 回收用户空间已映射的物理页
    uint32_t* pca=(uint32_t*)(satp&0xfffff000);
    for(uint32_t vpn=(0x1000>>12);vpn<(USER_START>>12);vpn++){
        uint32_t phys=vir2phy(pca,vpn<<12);
        if(phys){
            uint32_t ppn=(phys-RAM_START)>>12;  // 物理地址→页号
            free_page(ppn);
        }
        write_page_table(pca,vpn<<12,0,0);
    }
    free_page(((satp&0xfffff000)-RAM_START)>>12); // 释放页表目录页
    global_pcb_list[pid].is_alive=PROC_DEAD;
    shutdown();
    //TODO: 调度器就绪后, 改为切换到内核 shell 进程而非 shutdown
}

int get_free_pid(){
    for(uint32_t i=1;i<MAX_PRO_NUM;i++){
        if(global_pcb_list[i].is_alive==PROC_DEAD){
            return i;
        }
    }
    return -1;  // 无空闲 PID
}

uint32_t init_pcb(int new_pid,int stdout){
    global_pcb_list[new_pid].is_alive = PROC_RUNNING;
    global_pcb_list[new_pid].stdout=stdout;
    global_pcb_list[new_pid].ksp = 0x002f0000;  // 内核栈 (中断栈下方)
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
            // printk("program_start vir:%x\n",program_start_vir);
            uint8_t* program_start=(uint8_t*)vir2phy((uint32_t*)page_content_addr,(uint32_t)program_start_vir);//转换为物理地址；根据新satp转换，
            // printk("program_start:%x\n",program_start);
            readk(inode_id,file_data_buf,prog_head_offset,512);
            while(prog_head_filesize){
                printk("prog_head_filesize:%x\r",prog_head_filesize);
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

// uint32_t vmm_test(uint32_t page_content_addr,mnode* free_block_head){
//     char* tmp=(char*)mallock(30,(uint32_t*)page_content_addr,free_block_head);
//     printk("tmp:%x\n",tmp);
//     char* tmp1=(char*)mallock(30,(uint32_t*)page_content_addr,free_block_head);
//     printk("tmp1:%x\n",tmp1);
//     char* tmp2=(char*)mallock(30,(uint32_t*)page_content_addr,free_block_head);
//     printk("tmp2:%x\nfree tmp1",tmp2);
//     freek(tmp1,(uint32_t*)page_content_addr,free_block_head);
// }

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

// ============================================================
//  进程加载: 不跳转, 保存初始上下文到 PCB (供调度器首次 mret)
// ============================================================

int load_proc(uint32_t inode_id, char** para, uint32_t para_num) {
    int new_pid = get_free_pid();
    if (new_pid < 0) return -1;

    uint32_t page_content_addr = init_pcb(new_pid, -1);
    mnode* free_block_head = &(global_pcb_list[new_pid].free_block_head);

    uint32_t prog_start_addr = load_program(inode_id, page_content_addr, free_block_head);

    uint32_t* stack_top = (uint32_t*)mallock(0x100000, (uint32_t*)page_content_addr, free_block_head);
    uint32_t* stack_bottom = (uint32_t*)((void*)stack_top + 0xffffc);
    uint32_t argv = load_params(para_num, para, page_content_addr, free_block_head);

    // 保存初始 CPU 上下文到 PCB (模拟中断返回现场)
    pcb* p = &global_pcb_list[new_pid];
    p->pc = prog_start_addr;
    p->satp = page_content_addr | new_pid;
    for (int i = 0; i < 32; i++) p->general_regs[i] = 0;
    p->general_regs[2]  = (uint32_t)stack_bottom;  // sp
    p->general_regs[10] = para_num;                 // a0 = argc
    p->general_regs[11] = argv;                     // a1 = argv
    p->is_alive = PROC_READY;

    printk("load_proc: pid=%d entry=%x sp=%x satp=%x\n",
           new_pid, prog_start_addr, stack_bottom, p->satp);
    return new_pid;
}

// ============================================================
//  调度器基础设施
// ============================================================

pcb* current_pcb(void) {
    uint32_t satp;
    __asm__ volatile("csrrw %0, 0x182, zero" : "=r"(satp));
    __asm__ volatile("csrrw zero, 0x182, %0" :: "r"(satp));
    uint32_t pid = satp & 0x00000fff;
    if (pid == 0) return 0;
    return &global_pcb_list[pid];
}

// 宏展开: 逐个保存/恢复 regs_cp_m[i] (CSR 地址必须编译时常量)
void save_ctx_to_pcb(pcb* p) {
    if (p == 0) return;
    #define S(n) __asm__ volatile("csrrw %0, %1, zero" : "=r"(p->general_regs[n]) : "i"(0x3c0+n))
    S(0);S(1);S(2);S(3);S(4);S(5);S(6);S(7);
    S(8);S(9);S(10);S(11);S(12);S(13);S(14);S(15);
    S(16);S(17);S(18);S(19);S(20);S(21);S(22);S(23);
    S(24);S(25);S(26);S(27);S(28);S(29);S(30);S(31);
    #undef S
    __asm__ volatile("csrrw %0, 0x341, zero" : "=r"(p->pc));  // mepc
}

uint32_t restore_ctx_from_pcb(pcb* p) {
    #define R(n) __asm__ volatile("csrrw zero, %0, %1" :: "i"(0x3c0+n), "r"(p->general_regs[n]))
    R(0);R(1);R(2);R(3);R(4);R(5);R(6);R(7);
    R(8);R(9);R(10);R(11);R(12);R(13);R(14);R(15);
    R(16);R(17);R(18);R(19);R(20);R(21);R(22);R(23);
    R(24);R(25);R(26);R(27);R(28);R(29);R(30);R(31);
    #undef R
    return p->pc;
}

// ============================================================
//  调度器: 轮转, 返回下一个就绪进程的 pid (-1 表示无其他进程)
// ============================================================

int scheduler(void) {
    // 获取当前进程
    uint32_t satp;
    __asm__ volatile("csrrw %0, 0x182, zero" : "=r"(satp));
    uint32_t old_pid = satp & 0xfff;

    // 保存旧进程现场 (save_ctx_to_pcb 已保存 regs_cp_m + mepc)
    if (old_pid != 0 && global_pcb_list[old_pid].is_alive == PROC_RUNNING) {
        pcb* old_pcb = &global_pcb_list[old_pid];
        save_ctx_to_pcb(old_pcb);   // pcb->pc = mepc
        old_pcb->satp = satp;
        old_pcb->is_alive = PROC_READY;
    }

    // 轮转找下一个 READY
    static int last_pid = 1;
    int next_pid = -1;
    for (int i = 0; i < MAX_PRO_NUM; i++) {
        int pid = (last_pid + i) % MAX_PRO_NUM;
        if (pid == 0) continue;
        if (global_pcb_list[pid].is_alive == PROC_READY) {
            next_pid = pid;
            last_pid = pid;
            break;
        }
    }

    if (next_pid < 0) {
        // 无其他进程, 恢复当前
        __asm__ volatile("csrrw zero, 0x182, %0" :: "r"(satp));
        if (old_pid != 0) global_pcb_list[old_pid].is_alive = PROC_RUNNING;
        return -1;
    }

    // 恢复新进程
    pcb* next_pcb = &global_pcb_list[next_pid];
    next_pcb->is_alive = PROC_RUNNING;
    uint32_t new_mepc = restore_ctx_from_pcb(next_pcb);
    __asm__ volatile("csrrw zero, 0x341, %0" :: "r"(new_mepc));       // mepc
    __asm__ volatile("csrrw zero, 0x182, %0" :: "r"(next_pcb->satp)); // satp_i_cp

    // printk("[sched] %d->%d\n", old_pid, next_pid);
    return next_pid;
}

// ============================================================
//  定时器中断: 定义在 timerd.c (和 _regist_syscall 同文件, 避免 GOT 问题)
// ============================================================
//  初始进程 & 首次启动
// ============================================================

int init_ps(void) {
    for (int i = 1; i < MAX_PRO_NUM; i++) {
        global_pcb_list[i].is_alive = PROC_DEAD;
    }
    printk("init_ps: PCB table cleared\n");
    return 0;
}

void start_first_process(void) {
    pcb* p = &global_pcb_list[1];
    if (p->is_alive != PROC_READY) {
        printk("start_first_process: pid=1 not ready!\n");
        return;
    }
    p->is_alive = PROC_RUNNING;  // 标记为运行中, 防止调度器误判
    #define W(n) __asm__ volatile("csrrw zero, %0, %1" :: "i"(0x3c0+n), "r"(p->general_regs[n]))
    W(0);W(1);W(2);W(3);W(4);W(5);W(6);W(7);
    W(8);W(9);W(10);W(11);W(12);W(13);W(14);W(15);
    W(16);W(17);W(18);W(19);W(20);W(21);W(22);W(23);
    W(24);W(25);W(26);W(27);W(28);W(29);W(30);W(31);
    #undef W
    __asm__ volatile("csrrw zero, 0x341, %0" :: "r"(p->pc));
    __asm__ volatile("csrrw zero, 0x182, %0" :: "r"(p->satp));
    __asm__ volatile(".word 0x30200073");  // mret
}