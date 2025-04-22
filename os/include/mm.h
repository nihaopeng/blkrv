#ifndef _NVMM_H_
#define _NVMM_H_
#include "ini.h"
#include "math.h"
#include "std.h"
#include "set_gate.h"
#include "syscall.h"

// extern uint8_t blocks[MAX_BLOCK];

// extern uint8_t pages[MAX_PAGE];

typedef struct free_vir_block{
    uint32_t vir_addr;
    uint32_t size;//4B对齐
    struct free_vir_block* next;
};

void init_nvmm();

/*get a block number that is 0, 0 represents free
fail return -1*/
uint32_t alloc_block();

/*free a block
fail return -1*/
int free_block(uint16_t block_num);

//ram
void init_vmm();

uint32_t insert_pte(uint32_t satp_ppn,uint32_t new_pte);

uint32_t alloc_page();

uint32_t free_page(uint32_t page);

int freek(void* pointer,uint32_t satp_ppn);

void* mallock(uint32_t size,uint32_t satp_ppn);

int free_i(void* pointer);

void* malloc_i(uint32_t size);

void regist_free(int* dt_addr);

void regist_malloc(int* dt_addr);

int memset_s(char* addr,char ch,uint32_t count);

#endif // !_NVMM_H_
