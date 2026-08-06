#include "file.h"

// 统一 read: fd=0/1/2 是 TTY, 其余是文件 (通过进程 fd 表找到 inode)
int read_i(int fd,char* buf,uint32_t count){//we hope your buf has been init;
    _vir2phyk(char*,buf);
    fd_entry* e=fd_get_current(fd);
    if(!e||e->type==FD_EMPTY) return -1;
    if(e->type==FD_TTY){
        return tty_readk(buf,count);
    }
    uint32_t n=readk(e->inode_id,buf,e->offset,count);
    if(n>0) e->offset+=n;
    return n;
}

int write_i(int fd,char* buf,uint32_t count){
    _vir2phyk(char*,buf);
    fd_entry* e=fd_get_current(fd);
    if(!e||e->type==FD_EMPTY) return -1;
    if(e->type==FD_TTY){
        return tty_writek(buf,count);
    }
    int r=writek(e->inode_id,buf,e->offset,count);
    if(r<0) return r;
    e->offset+=count;
    return count;
}

int open_i(const char* file_path){
    _vir2phyk(char*,file_path);
    int inode_id=openk(file_path);
    if(inode_id<0) return -1;
    int fd=fd_alloc_current();
    if(fd<0) return -1;
    fd_entry* e=fd_get_current(fd);
    e->type=FD_FILE;
    e->inode_id=(uint32_t)inode_id;
    e->offset=0;
    return fd;
}

int create_i(char* file_path,char type,uint32_t* inode_id){
    _vir2phyk(char*,file_path);
    _vir2phyk(uint32_t*,inode_id);
    int id=createk(file_path,type,inode_id);
    if(id<0) return -1;
    int fd=fd_alloc_current();
    if(fd<0) return -1;
    fd_entry* e=fd_get_current(fd);
    e->type=FD_FILE;
    e->inode_id=(uint32_t)id;
    e->offset=0;
    return fd;
}

int finfo_i(int fd,inode* finode){// 按 fd 查询文件信息
    _vir2phyk(inode*,finode);
    int inode_id=fd_to_inode_current(fd);
    if(inode_id<0) return -1;
    return finfo_k((uint32_t)inode_id,finode);
}

int finoid_i(uint32_t inode_id,inode* finode){// 按 inode id 查询 (目录项场景)
    _vir2phyk(inode*,finode);
    return finfo_k(inode_id,finode);
}

int delete_i(int fd){
    int inode_id=fd_to_inode_current(fd);
    if(inode_id<0) return -1;
    return deletek(inode_id);
}

_regist_syscall(void,read);
_regist_syscall(void,write);
_regist_syscall(void,open);
_regist_syscall(void,create);
_regist_syscall(void,finfo);
_regist_syscall(void,finoid);
_regist_syscall(void,delete);
