#ifndef _FILE_H_
#define _FILE_H_

#include "ini.h"
#include "mm.h"
#include "str.h"
#include "math.h"
#include "set_gate.h"
#include "syscall.h"

typedef struct file
{
    /* data */
    char file_name[MAX_NAME];//max 128
    uint32_t size;
    uint32_t start_block;
    uint8_t type;
    //119 bit reserved
}inode;

int get_inode_by_id(uint32_t inode_id,inode** inode_get);

int delete_block_link(uint32_t start_block);

int find_file_in_dir(uint32_t inode_id,const char* name);

int create_inode(const char* file_name,char type);

int read_i(uint32_t inode_id,char* buf,uint32_t start,uint32_t count);

int write_i(uint32_t inode_id,char* buf,uint32_t count);

int create_i(const char* file_path,char type,uint32_t* inode_id);

int open_i(const char* file_path,uint32_t* inode_id,int* status);

// _syscall2(int,read,uint32_t,inode_id,char*,buf);
int read(uint32_t inode_id,char* buf,uint32_t start,uint32_t count);

// _syscall3(int,write,uint32_t,inode_id,char*,buf,uint32_t,length);
int write(uint32_t inode_id,char* buf,uint32_t count);

// _syscall3(int,create,uint32_t,dir_inode_id,char*,file_path,char,type);
int create(const char* file_path,char type,uint32_t* inode_id);

// _syscall1(uint32_t,open,const char*,file_path);
int open(const char* file_path,uint32_t* inode_id,int* status);

void regist_read(int* dt_addr);

void regist_write(int* dt_addr);

void regist_open(int* dt_addr);

void regist_create(int* dt_addr);

#endif // !_FILE_H_


