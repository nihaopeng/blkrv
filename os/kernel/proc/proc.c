#include "proc.h"

//在mmu不使能状态下执行，此时satp为0，因此系统调用应使用kernel函数
int exec_i(uint32_t inode_id,int stdout,char** para,uint32_t para_num){
    _vir2phyk(char**,para);
    for(int i=0;i<para_num;i++){
        _vir2phyk(char*,para[i]);
    }
    return execk(inode_id,stdout,para,para_num);
}

int exit_i(){
    exitk();
}

// 调度器已把目标进程上下文装入 REGS_CP_M / mepc / satp_i_cp,
// 直接从 ecall 处理器执行 mret 进入新进程 (永不返回)
static void __attribute__((noreturn)) switch_and_mret(void){
    __asm__ volatile(".word 0x30200073");  // mret
    for(;;){}  // 不可达
}

// waitpid: 阻塞等待子进程退出
int waitpid_i(int pid){
    uint32_t satp;
    __asm__ volatile("csrr %0,0x181":"=r"(satp));
    uint32_t my_pid = satp & 0xfff;

    if(pid <= 0 || pid >= MAX_PRO_NUM) return -1;
    if(global_pcb_list[pid].is_alive == PROC_DEAD) return 0;  // 已退出

    // 先保存上下文再标记 BLOCKED, 防止调度器跳过保存导致上下文丢失
    save_ctx_s_to_pcb(&global_pcb_list[my_pid]);
    global_pcb_list[my_pid].satp = satp;
    global_pcb_list[my_pid].wait_pid = pid;
    global_pcb_list[my_pid].is_alive = PROC_BLOCKED;
    int switched = scheduler();  // 立即调度, 让出 CPU
    if (switched < 0) {
        // 没有其他就绪进程: 恢复运行状态继续当前进程
        global_pcb_list[my_pid].is_alive = PROC_RUNNING;
        global_pcb_list[my_pid].wait_pid = 0;
        return 0;
    }
    // scheduler 已选好新进程并完成 mret 侧恢复, 移交 CPU
    switch_and_mret();
}

_regist_syscall(void,exec);
_regist_syscall(void,exit);
_regist_syscall(void,waitpid);
