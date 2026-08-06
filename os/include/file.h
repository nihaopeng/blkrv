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

int init_fs();

// int get_inode_by_id(uint32_t inode_id,inode** inode_get);

// int delete_block_link(uint32_t start_block);

// int find_file_in_dir(uint32_t inode_id,const char* name);

// int create_inode(const char* file_name,char type);

// int delete_inode(uint32_t inode_id);

int readk(uint32_t inode_id,char* buf,uint32_t start,uint32_t count);

int read_i(int fd,char* buf,uint32_t count);

int writek(uint32_t inode_id,char* buf,uint32_t start,uint32_t count);

int write_i(int fd,char* buf,uint32_t count);

int createk(char* file_path,char type,uint32_t* inode_id);

int create_i(char* file_path,char type,uint32_t* inode_id);

int openk(const char* file_path);

int open_i(const char* file_path);

int finfo_i(int fd,inode* finode);

int finfo_k(uint32_t inode_id,inode* finode);

int delete_i(int fd);

int deletek(uint32_t inode_id);

int finoid_i(uint32_t inode_id,inode* finode);

int read(int fd,char* buf,uint32_t count);

int write(int fd,const char* buf,uint32_t count);

int create(char* file_path,char type,uint32_t* inode_id);

int open(const char* file_path);

int finfo(int fd,inode* finode);

int finoid(uint32_t inode_id,inode* finode);

int delete(int fd);

void regist_finfo(int* dt_addr);

void regist_read(int* dt_addr);

void regist_write(int* dt_addr);

void regist_open(int* dt_addr);

void regist_create(int* dt_addr);

void regist_delete(int* dt_addr);
void regist_finoid(int* dt_addr);

//initialize the file system
int root();

#endif // !_FILE_H_
