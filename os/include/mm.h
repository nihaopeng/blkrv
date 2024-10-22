#ifndef _MM_H_
#define _MM_H_
#include "ini.h"
#include "math.h"

extern uint8_t blocks[MAX_BLOCK];

void load_blocks();

/*get a block number that is 0, 0 represents free
fail return -1*/
uint32_t alloc_block();

/*free a block
fail return -1*/
int free_block(uint16_t block_num);

int memset_s(char* addr,char ch,uint32_t count);

#endif // !_MM_H_
