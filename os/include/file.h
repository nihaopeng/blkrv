#ifndef _FILE_H_
#define _FILE_H_

#include "drivers.h"
#include "std.h"

typedef struct file {
    char file_name[MAX_NAME];
    uint32_t size;
    uint32_t start_block;
    uint8_t type;
} inode;

typedef struct dir_entry {
    char name[MAX_NAME];
    uint32_t inode_id;
    uint8_t type;
} dir_entry;

int init_fs();

// int get_inode_by_id(uint32_t inode_id,inode** inode_get);

// int delete_block_link(uint32_t start_block);

// int find_file_in_dir(uint32_t inode_id,const char* name);

// int create_inode(const char* file_name,char type);

// int delete_inode(uint32_t inode_id);

int readk(uint32_t inode_id,char* buf,uint32_t start,uint32_t count);

int read_i(uint32_t inode_id,char* buf,uint32_t start,uint32_t count);

int writek(uint32_t inode_id,char* buf,uint32_t start,uint32_t count);

int write_i(uint32_t inode_id,char* buf,uint32_t start,uint32_t count);

int createk(char* file_path,char type,uint32_t* inode_id);

int create_i(char* file_path,char type,uint32_t* inode_id);

int openk(const char* file_path);

int open_i(const char* file_path);

int read(uint32_t inode_id,char* buf,uint32_t start,uint32_t count);

int write(uint32_t inode_id,char* buf,uint32_t start,uint32_t count);

int create(char* file_path,char type,uint32_t* inode_id);

int open(const char* file_path);

inode* get_file_info(uint32_t inode_id);

void regist_read(int* dt_addr);

void regist_write(int* dt_addr);

void regist_open(int* dt_addr);

void regist_create(int* dt_addr);

//initialize the file system
int root();

#endif // !_FILE_H_


