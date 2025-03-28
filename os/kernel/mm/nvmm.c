#include "nvmm.h"

uint8_t blocks[MAX_BLOCK]={0};

void init_mm(){
    int* inst_4byte_addr=((int*)FILE_DATA_ADDR)+1023;
    for(int i=0;i<MAX_BLOCK;i++){
        if(((uint32_t)*inst_4byte_addr)>>28==BLOCK_USED)
            blocks[i]=1;
        else
            blocks[i]=0;
        inst_4byte_addr=inst_4byte_addr+1024;
    }
}

/*get a block number that is 0, 0 represents free
fail return -1*/
uint32_t alloc_block(){
    for(int i=0;i<MAX_BLOCK;i++){
        if(!blocks[i]){
            int* addr=((int*)(((void*)FILE_DATA_ADDR)+mul(i,BLOCK_SIZE)))+1023;
            *addr=0x10000000;
            blocks[i]=1;
            return i;
        }
    }
    return -1;   
}

/*free a block
fail return -1*/
int free_block(uint16_t block_num){
    // printk("free block:%d\n",block_num);
    if(blocks[block_num]){
        blocks[block_num]=0;
        int* addr=((int*)(((void*)FILE_DATA_ADDR)+mul(block_num,BLOCK_SIZE)))+1023;
        *addr=0;
    }
    else
        return -1;
    return 0;
}

int memset_s(char* addr,char ch,uint32_t count){
    for(int i=0;i<count;i++){
        *(addr) = ch;
        addr+=1;
    }
    return 0;
}