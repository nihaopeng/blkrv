#include "file.h"

void init_filetree(){
    
}

file* get_inode_by_id(uint32_t id){
    file* file_tmp;
    uint32_t inode_addr=file_table_addr+id<<8;
    uint32_t size,start_block;
    uint8_t type;
    __asm__ volatile (
        "li a0,%3\n"
        "lw %0,128(a0)\n"
        "lw %1,132(a0)\n"
        "lb %2,136(a0)\n"
        :"=r"(size),"=r"(start_block),"=r"(type)
        :"r"(inode_addr)
    );
    file_tmp->file_name=(char*)inode_addr;
    file_tmp->size=size;
    file_tmp->start_block=start_block;
    file_tmp->type=type;
    return file_tmp;
}

void read(uint32_t inode_id,char* buf){
    file* file_tmp=get_inode_by_id(inode_id);
    
}

uint32_t get_id_in_block_by_name(uint32_t block,char* name){
    uint32_t id_tmp;
    char* name_tmp;
    
    __asm__ volatile(
        "\n"
    );
}

int open(const char* file_path){

}

