#include "file.h"

int read_i(uint32_t inode_id,char* buf,uint32_t start,uint32_t count){//we hope your buf has been init;
    _vir2phyk(char*,buf);

    return readk(inode_id,buf,start,count);
}

int write_i(uint32_t inode_id,char* buf,uint32_t start,uint32_t count){//the start should not bigger than size
    _vir2phyk(char*,buf);

    return writek(inode_id,buf,start,count);
}

int open_i(char* file_path,uint32_t* inode_id){
    _vir2phyk(char*,file_path);
    _vir2phyk(uint32_t*,inode_id);
    // _vir2phyk(int*,status);

    return openk(file_path,inode_id);
}

int create_i(char* file_path,char type,uint32_t* inode_id){
    _vir2phyk(char*,file_path);
    _vir2phyk(uint32_t*,inode_id);
    // _vir2phyk(int*,status);

    return createk(file_path,type,inode_id);
}

_regist_syscall(void,read);
_regist_syscall(void,write);
_regist_syscall(void,open);
_regist_syscall(void,create);

