#include "mm.h"

uint8_t blocks[MAX_BLOCK]={0};

/*get a block number that is 0, 0 represents free
fail return -1*/
uint32_t alloc_block(){
    for(int i=0;i<MAX_BLOCK;i++){
        if(!blocks[i]){
            return i;
        }
    }
    return -1;   
}

/*free a block
fail return -1*/
int free_block(uint16_t block_num){
    if(blocks[block_num])
        blocks[block_num]=0;
    else
        return -1;
    return 0;
}

int memset_i(char* addr,char ch){
    char *a = (addr);
    *(a) = (ch);
    return 0;
}