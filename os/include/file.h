#include "ini.h"

uint32_t file_table_addr=0x100200;

uint8_t files[4096]={0};

typedef struct file
{
    /* data */
    char* file_name;//max 128
    uint32_t size;
    uint32_t start_block;
    uint8_t type;
    //24 bit reserved
}file;

file* get_inode_by_id(uint32_t id);

uint32_t get_id_in_block_by_name(uint32_t block,char* name);

void read(uint32_t inode_id,char* buf);

int create(const char* file_path);

int open(const char* file_path);