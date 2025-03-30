#include "file.h"

int read_i(uint32_t inode_id,char* buf,uint32_t start,uint32_t count){//we hope your buf has been init;
    _vir2phy(char*,buf);

    readk(inode_id,buf,start,count);
}

int write_i(uint32_t inode_id,char* buf,uint32_t start,uint32_t count){//the start should not bigger than size
    _vir2phy(char*,buf);

    writek(inode_id,buf,start,count);
}

int open_i(char* file_path,uint32_t* inode_id,int* status){
    _vir2phy(char*,file_path);
    _vir2phy(uint32_t*,inode_id);
    _vir2phy(int*,status);

    openk(file_path,inode_id,status);
}

int create_i(char* file_path,char type,uint32_t* inode_id,int* status){
    _vir2phy(char*,file_path);
    _vir2phy(uint32_t*,inode_id);
    _vir2phy(int*,status);

    createk(file_path,type,inode_id,status);
}

_regist_syscall(void,read);
_regist_syscall(void,write);
_regist_syscall(void,open);
_regist_syscall(void,create);

