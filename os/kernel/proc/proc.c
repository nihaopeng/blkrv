#include "proc.h"

uint8_t pro_ids[MAX_PID_NUM]={0};
pcb global_pcb_list[64];

int init_ps(){
    pro_ids[0]=1;
    global_pcb_list[0].pid=0;
    global_pcb_list[0].virtual_base_addr=0x00000000;
    global_pcb_list[0].stdout=-1;
    global_pcb_list[0].stdout_start=0;
}

int exec(uint32_t inode_id,int priority,int stdout,int stdout_start,int* pid,int* status,char* para[]){
    __asm__ volatile (
        "mv %0,ra"
        :"=r"(global_pcb_list[0].pc_reg)
        :
    );
    // printk("kernel exec pc:%d",global_pcb_list[0].pc_reg);
    inode* ino;
    get_inode_by_id(inode_id,&ino);
    uint32_t i;
    for(i=1;i<MAX_PID_NUM;i++){
        if(!pro_ids[i]){
            global_pcb_list[i].pid=i;
            global_pcb_list[i].virtual_base_addr=0x10000000+(i<<23);
            global_pcb_list[i].heap_addr=0x10000000+(i<<23)+4*1024*1024;
            pro_ids[i]=1;
            goto flag;
        }
    }
    printk("num of proc reaches limitaion\n");
    *status=-1;
    return -1;
flag:
    global_pcb_list[i].stdout=stdout;
    global_pcb_list[i].stdout_start=stdout_start;
    //load prog
    char buf[1024];
    memset_s(buf,0,1024);
    int j=0;
    printk("load prog...\n");
    char* st=(char*)global_pcb_list[i].virtual_base_addr;
    while(read_i(inode_id,buf,j,1024)!=-1){
        for(int k=0;k<1024;k++){
            *st=buf[k];
            // printk("%d,",*st);
            st+=1;
        }
        printk("%d\r",j);
        j+=1024;
    }
    printk("start pcb(std_out=%d)\n",global_pcb_list[i].stdout);
    set_stdout(global_pcb_list[i].stdout,global_pcb_list[i].stdout_start);
    printk("enter_prog,va=%d\n",global_pcb_list[i].virtual_base_addr);
    enter_prog(global_pcb_list[i].virtual_base_addr);
    *status=0;
}

int exit_i(){
    //回收资源，善后工作
    uint32_t p=0;
    __asm__ volatile(
        "csrr %0,0x181\n"
        //将satp置0
        "csrw 0x181,0\n"
        :"=r"(p)
    );
    p=(p<<4)>>4;//去除mmu以及dev标志位
    uint32_t pid=p>>23;
    printk("pid:%d\n",pid);
    pro_ids[pid]=0;
    
    //跳转调度器
    scheduler(pid);
}


//TODO; better scheduler
int scheduler(int pid){
    for(int i=MAX_PID_NUM-1;i>=1;i--){
        if(pro_ids[i]){
            set_stdout(global_pcb_list[pid].stdout,global_pcb_list[pid].stdout_start);
            __asm__ volatile(
                "lui a0,0x80000\n"
                "add a1,%0,a0\n"
                "csrw satp,a1\n"
                "jalr zero,%1\n"
                :
                :"r"(global_pcb_list[i].virtual_base_addr),"r"(global_pcb_list[i].pc_reg)
            );
        }
    }
    shutdown();
}

void regist_exit(int* gdt_addr_exit){
    int* func_addr_exit=(int*)(&exit_i);
    _set_gate(gdt_addr_exit,func_addr_exit);
}