#include "file.h"

//通过id获取结构体inode
int get_inode_by_id(uint32_t inode_id,inode** inode_get){
    //获取对应inode的地址，hash索引
    void* inode_addr=(void*)FILE_TABLE_ADDR+mul(INODE_SIZE,inode_id);
    *inode_get=(inode*)inode_addr;
}

//删除从startblock开始的block链表
int delete_block_link(uint32_t start_block){
    int* inst_4byte_addr=(int*)(((void*)FILE_DATA_ADDR)+mul(start_block,BLOCK_SIZE));
    uint32_t cur_block=start_block;
    while(1){
        uint32_t next=(((uint32_t)*inst_4byte_addr)>>4)<<4;
        if(next==0)
            break;
        else{
            free_block(cur_block);
            cur_block=next;
            inst_4byte_addr=(int*)(((void*)FILE_DATA_ADDR)+mul((int)(cur_block),BLOCK_SIZE));
        }
    }
}

int find_file_in_dir(uint32_t inode_id,const char* name){
    char files[1024];
    int i=0;
    inode* ino;
    while(read_i(inode_id,files,i,1024)!=-1){
        i+=1024;
        for(int j=0;j<1024;j+=4){
            get_inode_by_id(*(int*)(&files[j]),&ino);
            if(str_cmp(ino->file_name,name)){
                return *(int*)(&files[j]);
            }
        }
    }
    return -1;
}

int create_inode(const char* file_name,char type){
    inode* tmp=(inode*)FILE_TABLE_ADDR;
    for(int i=0;i<FILE_NUM;i++){
        if(tmp->type==0){
            str_cpy(file_name,tmp->file_name);
            tmp->size=0;
            tmp->start_block=alloc_block();
            tmp->type=(uint8_t)type;
            return i;
        }else{
            tmp+=1;
        }
    }
    return -1;
}

int read_i(uint32_t inode_id,char* buf,uint32_t start,uint32_t count){//we hope your buf has been init;
    inode* ino;
    get_inode_by_id(inode_id,&ino);
    if(start>ino->size){
        return -1;
    }
    char* addr=(char*)(((void*)FILE_DATA_ADDR)+mul(ino->start_block,BLOCK_SIZE));
    int tmp=0;
    int cnt=0;
    for(int i=0;i<count;i++){
        if(mod(tmp++,4092)==0){
            uint32_t next=*((uint32_t*)addr);
            if(next>>28==0)break;//if nas not been used
            addr=(char*)(((void*)FILE_DATA_ADDR)+mul(((next<<4)>>4),BLOCK_SIZE));//calc addr of next block;
        }
        if(count==start){
            *buf=*addr;buf+=1;
        }
        addr+=1;//get data;
    }
    *buf='\0';
    return 0;
}

int write_i(uint32_t inode_id,char* buf,uint32_t count){
    inode* ino=NULL;
    get_inode_by_id(inode_id,&ino);
    //delete primitive block
    delete_block_link(ino->start_block);
    //write data
    uint32_t srtblk=alloc_block();
    ino->start_block=srtblk;
    char* addr=(char*)(((void*)FILE_DATA_ADDR)+mul(srtblk,BLOCK_SIZE));
    int tmp=0;
    int cnt=0;
    while(count--){
        if(mod(tmp++,4092)==0){
            uint32_t new_block=alloc_block();
            *(int*)addr+=new_block;//设置指向的下一个block;
            addr=(char*)(((void*)FILE_DATA_ADDR)+mul(new_block,BLOCK_SIZE));//calc addr of next block;
        }
        *addr=*buf;buf+=1;addr+=1;//put data;
    }
    ino->size=count;
}

int open_i(const char* file_path,uint32_t* inode_id,int* status){
    //解析地址
    char file_path_p[129];
    if(file_path[str_len(file_path)-1]!='/'){
        str_cpy(file_path,file_path_p);
        file_path_p[str_len(file_path_p)]='/';
    }
    uint32_t file_path_len=str_len(file_path_p);
    char stack[128];
    int stack_ptr=0;
    uint32_t cur_inode_id=0;
    for(int i=1;i<file_path_len;i++){
        if(file_path[i]=='/'){
            stack[stack_ptr]='\0';
            int in=find_file_in_dir(cur_inode_id,stack);
            if(in!=-1)
                cur_inode_id=in;
            else{
                *status=-1;
                return 0;
            }
            stack_ptr=0;
        }else{
            stack[stack_ptr++]=file_path_p[i];
        }
    }
    inode* ino;
    get_inode_by_id(cur_inode_id,&ino);
    if(ino->type!=0){
        *inode_id=cur_inode_id;
        *status=0;
    }else{
        *status=-1;
    }
    return 0;
}

int create_i(const char* file_name,char type,uint32_t* inode_id){
    *inode_id=create_inode(file_name,type);
}

_syscall4(int,read,uint32_t,inode_id,char*,buf,uint32_t,start,uint32_t,count);
_syscall3(int,write,uint32_t,inode_id,char*,buf,uint32_t,length);
_syscall3(int,create,const char*,file_path,char,type,uint32_t*,inode_id);
_syscall3(int,open,const char*,file_path,uint32_t*,inode_id,int*,status);

void regist_read(int* dt_adrr){
    int* func_addr_read=(int*)(&read_i);
    _set_gate(dt_adrr,func_addr_read);
}

void regist_write(int* dt_adrr){
    int* func_addr_write=(int*)(&write_i);
    _set_gate(dt_adrr,func_addr_write);
}

void regist_open(int* dt_adrr){
    int* func_addr_open=(int*)(&open_i);
    _set_gate(dt_adrr,func_addr_open);
}

void regist_create(int* dt_adrr){
    int* func_addr_create=(int*)(&create_i);
    _set_gate(dt_adrr,func_addr_create);
}