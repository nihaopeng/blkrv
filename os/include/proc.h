#ifndef _PROC_H_
#define _PROC_H_
#include "file.h"
#include "drivers.h"

#define debugk(a) printk("%s:%x\n",#a,a)

typedef struct mnode{
    uint32_t size;//4B对齐
    struct mnode* next;
}mnode;

// 每个进程的打开文件表项
typedef struct fd_entry{
    uint32_t type;      // FD_EMPTY / FD_TTY / FD_FILE
    uint32_t inode_id;  // FD_FILE 时指向全局 inode 表下标
    uint32_t offset;    // 当前读写位置
}fd_entry;

typedef struct pcb{
    uint8_t is_alive;           // PROC_RUNNING/READY/BLOCKED/DEAD
    uint8_t parent_pid;         // 父进程 pid (进程树, exitk 递归杀子)
    uint8_t wait_pid;           // 等待的子进程 pid, 0=不等待
    uint32_t pc;                // saved mepc
    uint32_t general_regs[32];  // saved regs_cp_m snapshot
    uint32_t satp;              // page table base | pid
    uint32_t ksp;               // kernel stack (reserved)
    mnode free_block_head;
    fd_entry fdt[MAX_FD];       // 打开文件表: 0/1/2 预置为 TTY
}pcb;

extern pcb global_pcb_list[MAX_PRO_NUM];
extern uint32_t syscall_active_pid;  // _gdt_search 设置, 调度器在 syscall 中识别当前进程

int init_ps();

void save_ctx_to_pcb(pcb* p);
void save_ctx_s_to_pcb(pcb* p);

pcb* current_pcb(void);

// fd 表操作 (放在 prock.c, 避免跨文件访问 global_pcb_list 走 GOT)
void fd_init(pcb* p);
fd_entry* fd_get(pcb* p,int fd);
int fd_alloc(pcb* p);
fd_entry* fd_get_current(int fd);
int fd_alloc_current(void);
int fd_to_inode_current(int fd);

void recover_contxt();

void enter_prog();

int exit_i();

int exitk();

int exit();

int exec_i(uint32_t inode_id,int stdout,char** para,uint32_t para_num);

int execk(uint32_t inode_id,int stdout,char** para,uint32_t para_num);

int exec(uint32_t inode_id,int stdout,char** para,uint32_t para_num);

int scheduler();

int spawn_i(int fd, char** para, uint32_t para_num);

int waitpid_i(int pid);

int spawn(int fd, char** para, uint32_t para_num);

int waitpid(int pid);

int load_proc(uint32_t inode_id, char** para, uint32_t para_num);

void start_first_process(void);

void timer_interrupt_i(void);

void regist_exec(int* gdt_addr_exec);

void regist_exit(int* gdt_addr_exit);
void regist_spawn(int* dt_addr);
void regist_waitpid(int* dt_addr);

void init_vmm();

uint32_t alloc_page();

uint32_t free_page(uint32_t page);

uint32_t write_page_table(uint32_t* page_content_addr,uint32_t vir_addr,uint32_t phy_addr,uint32_t mode);

uint32_t vir2phy(uint32_t* page_content_addr,uint32_t vir_addr);

uint32_t show_free_node_list(uint32_t* page_content_addr,mnode* free_block_head);

int freek(void* pointer,uint32_t* page_content_addr,mnode* free_block_head);

void* mallock(uint32_t size,uint32_t* page_content_addr,mnode* free_block_head);

int free_i(void* pointer);

void* malloc_i(uint32_t size);

int free(void* pointer);

void* malloc(uint32_t size);

void regist_free(int* dt_addr);

void regist_malloc(int* dt_addr);

int memset_s(char* addr,char ch,uint32_t count);

#endif // !_PROC_H_
