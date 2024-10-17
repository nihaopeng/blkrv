#ifndef _FILE_H_
#define _FILE_H_

#include "ini.h"
#include "mm.h"
#include "str.h"
#include "set_gate.h"
#include "syscall.h"

uint8_t FILES[4096]={0};

typedef struct file
{
    /* data */
    char file_name[MAX_NAME];//max 128
    uint32_t size;
    uint32_t start_block;
    uint8_t type;
    //24 bit reserved
}inode;

uint32_t alloc_inode();

void delete_inode(uint32_t inode_id);

inode* get_inode_by_id(uint32_t id);

int get_id_by_name(uint32_t inode_id,char* name);

void read_i(uint32_t inode_id,char* buf);

void write_i(uint32_t inode_id,char* buf,uint32_t length);

int create_i(uint32_t dir_inode,char* file_path,char type);

int open_i(const char* file_path,inode* inode);

void init_fs();

_syscall2(int,read,uint32_t,inode_id,char*,buf);

_syscall3(int,write,uint32_t,inode_id,char*,buf,uint32_t,length);

_syscall3(int,create,uint32_t,dir_inode_id,char*,file_path,char,type);

_syscall1(uint32_t,open,const char*,file_path);

#endif // !_FILE_H_


