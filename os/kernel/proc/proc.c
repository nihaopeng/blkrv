#include "proc.h"

// uint32_t cur_pro=0;
uint8_t pro_ids[MAX_PID_NUM]={0};
pcb global_pcb_list[64];

int init_ps(){
    global_pcb_list[0].pid=0;
    global_pcb_list[0].virtual_base_addr=0x00000000;
    global_pcb_list[0].stdout=0;
    global_pcb_list[0].stdout_start=0;
}

// void enter_prog(uint32_t ram_start_addr){
//     printk("test");
//     __asm__ volatile (
//         "mv a1,a0\n"
//         "addi a0,a0,%0\n"
//         "lw a0,0(a0)\n"
//         "li a2,0xffff0000\n"
//         "add a0,a2,a0\n"
//         "addi a2,zero,0\n"
//         "lui a2,0x80000\n"
//         "add a1,a2,a1\n"
//         "csrrw zero,satp,a1\n"
//         //test
//         "jalr zero,0(a0)\n"
//         :
//         :"i"(0x18)
//     );
// }

int exec_i(uint32_t inode_id,int priority,int stdout,int stdout_start,int* pid,int* status,char* para[]){
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
    // global_pcb_list[i].priority=priority;
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
    printk("start pcb...\n");
    start_pcb(i);
    *status=0;
}

int start_pcb(int pid){
    // cur_pro=pid;
    set_stdout(global_pcb_list[pid].stdout,global_pcb_list[pid].stdout_start);
    printk("enter_prog,%d\n",global_pcb_list[pid].virtual_base_addr);
    // __asm__ volatile("csrw satp,a0"::);
    enter_prog(global_pcb_list[pid].virtual_base_addr);
}

// void* user_to_global(void* ptr){
//     return (void*)(ptr+global_pcb_list[cur_pro].virtual_base_addr);
// }