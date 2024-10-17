#include "file.h"

uint32_t alloc_inode(){
    for(int i=0;i<4096;i++){
        if(!FILES[i]){
            FILES[i]=1;
            return i;
        }
    }
    return 0;
}

void delete_inode(uint32_t inode_id){

}

void init_fs(){
    _set_gate(_NR_read,&read_i);
    _set_gate(_NR_write,&write_i);
    _set_gate(_NR_open,&open_i);
    _set_gate(_NR_create,&create_i);
}

inode* get_inode_by_id(uint32_t id){
    inode* file_tmp=(inode*)(FILE_TABLE_ADDR+id<<8);
    // uint32_t size,start_block;
    // uint8_t type;
    // __asm__ volatile (
    //     "li a0,%3\n"
    //     "lw %0,128(a0)\n"
    //     "lw %1,132(a0)\n"
    //     "lb %2,136(a0)\n"
    //     :"=r"(size),"=r"(start_block),"=r"(type)
    //     :"r"(inode_addr)
    // );
    // file_tmp->file_name=(char*)inode_addr;
    // file_tmp->size=size;
    // file_tmp->start_block=start_block;
    // file_tmp->type=type;
    return file_tmp;
}

void read_i(uint32_t inode_id,char* buf){//we hope your buf has been init;
    inode* file_tmp=get_inode_by_id(inode_id);
    uint32_t block_tmp=file_tmp->start_block;
    int block_num=file_tmp->size/(BLOCK_SIZE)+1;
    uint32_t pos=0;
    for(int i=0;i<block_num;i++){
        uint32_t j=0;
        for(;j<BLOCK_SIZE;j++){
            buf[pos++]=*(char*)(FILE_DATA_ADDR+block_tmp<<12+j);
        }
        j++;
        block_tmp=*(int*)(FILE_DATA_ADDR+block_tmp<<12+j);
    }
    buf[pos]='\0';
}

void write_i(uint32_t inode_id,char* buf,uint32_t length){//未实现清除原有block的功能；
    inode* file_tmp=get_inode_by_id(inode_id);
    uint32_t block_tmp=file_tmp->start_block;
    int block_num=length/(BLOCK_SIZE)+1;
    uint32_t pos=0;
    for(int i=0;i<block_num;i++){
        uint32_t j=0;
        for(;j<BLOCK_SIZE;j++){
            *(char*)(FILE_DATA_ADDR+block_tmp<<12+j)=buf[pos++];
        }
        j++;
        block_tmp=alloc_block();
        *(int*)(FILE_DATA_ADDR+block_tmp<<12+j)=block_tmp;
    }
    file_tmp->size=length;
}

int get_id_by_name(uint32_t inode_id,char* name){
    uint32_t id_tmp;
    char data[MAX_NAME];
    read(inode_id,data);
    uint32_t leaf_inode_id;
    for(uint32_t i=0;i<str_len(data);i+=4){
        leaf_inode_id=*(const uint32_t*)&data[i];
        if(str_cmp(get_inode_by_id(leaf_inode_id)->file_name,name)){
            return leaf_inode_id;
        }
    }
    return -1;
}

int open_i(const char* file_path,inode* inode){
    uint32_t cur_inode_id=0;
    uint32_t path_len=str_len(file_path);
    char tmp[MAX_NAME];
    uint32_t start=1;
    for(int i=1;i<path_len;i++){
        if(file_path[i]=='/'){//separate;
            str_cpy_s(file_path,tmp,start,i-1);
            start=i+1;
            uint32_t cur_inode_id=get_id_by_name(cur_inode_id,tmp);
        }else if(i==path_len-1){//leaf file
            str_cpy_s(file_path,tmp,start,i-1);
            uint32_t cur_inode_id=get_id_by_name(cur_inode_id,tmp);
        }
        if(cur_inode_id==-1){//file not exist
            return -1;
        }
    }
    //the cur_inode is leaf
    inode=get_inode_by_id(cur_inode_id);
    return 0;
}

int create_i(uint32_t dir_inode, char* file_name,char type){
    //写文件夹内容
    char data[MAX_NAME];
    read(dir_inode,data);
    uint32_t data_len=str_len(data);
    uint32_t new_inode=alloc_inode();
    *(uint32_t*)&data[data_len]=new_inode;
    write(dir_inode,data,data_len+4);
    //增加inode节点
    inode* file_tmp=(inode*)(FILE_TABLE_ADDR+new_inode<<8);
    str_cpy(file_tmp->file_name,file_name);
    file_tmp->size=0;
    file_tmp->start_block=alloc_block();
    file_tmp->type=type;
}
