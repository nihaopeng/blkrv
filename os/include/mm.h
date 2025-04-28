#ifndef _NVMM_H_
#define _NVMM_H_
#include "ini.h"
#include "math.h"
#include "std.h"
#include "set_gate.h"
#include "syscall.h"

// extern uint8_t blocks[MAX_BLOCK];

// extern uint8_t pages[MAX_PAGE];

typedef struct mnode{
    uint32_t size;//4B对齐
    struct mnode* next;
}mnode;

void init_nvmm();

/*get a block number that is 0, 0 represents free
fail return -1*/
uint32_t alloc_block();

/*free a block
fail return -1*/
int free_block(uint16_t block_num);

//ram
void init_vmm();

uint32_t alloc_page();

uint32_t free_page(uint32_t page);

uint32_t write_page_table(uint32_t* page_content_addr,uint32_t vir_addr,uint32_t phy_addr,uint32_t mode);

uint32_t read_page_table(uint32_t* page_content_addr,uint32_t vir_addr);

uint32_t show_free_node_list(uint32_t* page_content_addr);

int freek(void* pointer,uint32_t* page_content_addr);

void* mallock(uint32_t size,uint32_t* page_content_addr);

int free_i(void* pointer);

void* malloc_i(uint32_t size);

int free(void* pointer);

void* malloc(uint32_t size);

void regist_free(int* dt_addr);

void regist_malloc(int* dt_addr);

int memset_s(char* addr,char ch,uint32_t count);

#endif // !_NVMM_H_
