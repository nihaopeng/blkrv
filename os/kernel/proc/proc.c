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

int exec(uint32_t inode_id,int priority,int stdout,int stdout_start,int* pid,int* status,char** para,uint32_t para_num){
    //暂时保存在0，后续考虑是否加入current pid
    uint32_t ra=0;
    __asm__ volatile (
        "mv %0,ra"
        :"=r"(ra)
        :
    );
    save_contxt(global_pcb_list[0].context_reg,global_pcb_list[0].context_csr,&(global_pcb_list[0].pc_reg),ra);
    // printk("kernel exec pc:%d",global_pcb_list[0].pc_reg);
    inode* ino;
    get_inode_by_id(inode_id,&ino);
    uint32_t i;
    for(i=1;i<MAX_PID_NUM;i++){
        if(!pro_ids[i]){
            global_pcb_list[i].pid=i;
            global_pcb_list[i].virtual_base_addr=0x10000000+PROC_MEM;
            global_pcb_list[i].heap_addr=0x10000000+PROC_MEM+PROC_MEM>>1;
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
    while(readk(inode_id,buf,j,1024)!=-1){
        for(int k=0;k<1024;k++){
            *st=buf[k];
            // printk("%d,",*st);
            st+=1;
        }
        printk("%d\r",j);
        j+=1024;
    }
    // printk("start pcb(std_out=%d)\n",global_pcb_list[i].stdout);
    set_stdout(global_pcb_list[i].stdout,global_pcb_list[i].stdout_start);
    // printk("enter_prog,va=%d\n",global_pcb_list[i].virtual_base_addr);

    //load para
    void* virtual_base_addr=(void*)global_pcb_list[i].virtual_base_addr;
    void* stack_addr=(void*)((void*)global_pcb_list[i].virtual_base_addr+PROC_MEM)-4;
    char* para_addr=stack_addr-(para_num<<2)-1;//参数数据存放的末地址
    for(int i=para_num-1;i>=0;i--,stack_addr-=4){
        // printk("para_addr:%d,vir:%d\n",para_addr,virtual_base_addr);
        printk("para:i:%d,len:%d,data:%s\n",i,str_len(para[i]),para[i]);
        int para_len=str_len(para[i]);
        for(int j=para_len-1;j>=0;j--){
            *(para_addr--)=para[i][j];//将参数数据存到用户栈
        }
        *(uint32_t*)stack_addr=(uint32_t)(para_addr+1-(char*)virtual_base_addr);//将参数指针存储到用户栈
        printk("para_addr:%d,arg_addr:%d\n",para_addr,*(uint32_t*)stack_addr);
        *(para_addr--)='\0';//分隔字符串
    }
    para=(char**)(stack_addr+4-virtual_base_addr);
    printk("para:%d\n",para);
    enter_prog(global_pcb_list[i].virtual_base_addr,para,para_num,\
    ((((uint32_t)para_addr-1-4)>>2)<<2)-(uint32_t)virtual_base_addr);//((para_addr+4)>>2)<<2用来对齐字节
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
    // printk("pid:%d\n",pid);
    pro_ids[pid]=0;
    
    //跳转调度器
    scheduler();
}


//TODO; better scheduler
int scheduler(){
    for(int i=MAX_PID_NUM-1;i>=0;i--){
        if(pro_ids[i]){
            printk("run pid:%d\n",i);
            set_stdout(global_pcb_list[i].stdout,global_pcb_list[i].stdout_start);
            printk("recover context\n");
            recover_contxt(global_pcb_list[i].context_reg,global_pcb_list[i].context_csr,global_pcb_list[i].pc_reg);
        }
    }
}

void regist_exit(int* gdt_addr_exit){
    int* func_addr_exit=(int*)(&exit_i);
    _set_gate(gdt_addr_exit,func_addr_exit);
}