#include "file.h"

int init_fs(){
    uint32_t inode_id;
    int status;
    create_i("/",'d',&inode_id,&status);
    create_i("/include",'d',&inode_id,&status);
    create_i("/tmp",'d',&inode_id,&status);
    create_i("/tmp/test.bin",'f',&inode_id,&status);
}

//通过id获取结构体inode
int get_inode_by_id(uint32_t inode_id,inode** inode_get){
    //获取对应inode的地址，hash索引
    void* inode_addr=(void*)FILE_TABLE_ADDR+mul(INODE_SIZE,inode_id);
    *inode_get=(inode*)inode_addr;
}

//删除从startblock开始的block链表
int delete_block_link(uint32_t start_block){
    // printk("delete start block:%d\n",start_block);
    int* inst_4byte_addr=(int*)(((void*)FILE_DATA_ADDR)+mul(start_block,BLOCK_SIZE))+1023;
    uint32_t cur_block=(((uint32_t)*inst_4byte_addr)<<4)>>4;
    *inst_4byte_addr=0x10000000;
    while(1){
        if(cur_block==0)
            break;
        else{
            inst_4byte_addr=(int*)(((void*)FILE_DATA_ADDR)+mul((int)(cur_block),BLOCK_SIZE))+1023;
            free_block(cur_block);
            uint32_t next=(((uint32_t)*inst_4byte_addr)<<4)>>4;
            cur_block=next;
        }
    }
}

int find_file_in_dir(uint32_t inode_id,const char* name){
    char files[1024];
    memset_s(files,0,1024);
    int i=0;
    inode* ino;
    while(readk(inode_id,files,i,1024)!=-1){
        for(int j=0;j<1024;j+=4){
            get_inode_by_id(*(uint32_t*)(&files[j]),&ino);
            if(str_cmp(ino->file_name,name)){
                return *(int*)(&files[j]);
            }
        }
        i+=1024;
    }
    return -1;
}

int create_inode(const char* file_name,char type){
    inode* tmp=(inode*)FILE_TABLE_ADDR;
    for(int i=0;i<FILE_NUM;i++){
        if(tmp->type==0){
            // print("filename:%s\n",file_name);
            str_cpy(file_name,tmp->file_name);
            // print("filename:%s\n",tmp->file_name);
            tmp->size=0;
            tmp->start_block=alloc_block();
            // print("start_b:%d\n",tmp->start_block);
            tmp->type=(uint8_t)type;
            // print("type:%d\n",tmp->type);
            return i;
        }else{
            tmp=(inode*)((void*)tmp+INODE_SIZE);
            // print("tmpPtr:%d\n",tmp);
        }
    }
    return -1;
}

int delete_inode(uint32_t inode_id){
    inode* ino=(inode*)((void*)FILE_TABLE_ADDR+mul(inode_id,INODE_SIZE));
    memset_s(ino->file_name,0,MAX_NAME);
    ino->type=0;
    delete_block_link(ino->start_block);
    ino->start_block=0;
    ino->size=0;
}

int readk(uint32_t inode_id,char* buf,uint32_t start,uint32_t count){//we hope your buf has been init;
    inode* ino;
    get_inode_by_id(inode_id,&ino);
    if(start>=ino->size){
        return -1;
    }
    char* addr=(char*)(((void*)FILE_DATA_ADDR)+mul(ino->start_block,BLOCK_SIZE));
    int tmp=1;
    int cnt=0;
    for(int i=0;i<ino->size;i++){
        if(mod(tmp++,4093)==0){
            uint32_t next=*((uint32_t*)addr);
            addr=(char*)(((void*)FILE_DATA_ADDR)+mul(((next<<4)>>4),BLOCK_SIZE));//calc addr of next block;
            i--;
            continue;
        }
        else if(i>=start){
            *buf=*addr;
            buf+=1;
            cnt+=1;
        }
        if(cnt>=count){
            // printk("\n%d,%d",cnt,count);
            break;
        }
        addr+=1;//get data;
    }
    *buf='\0';
    return 0;
}

int read_i(uint32_t inode_id,char* buf,uint32_t start,uint32_t count){//we hope your buf has been init;
    uint32_t p=0;
    __asm__ volatile(
        "csrr %0,0x181"
        :"=r"(p)
    );
    p=(p<<1)>>1;//去除mmu标志位
    // printk("%d\n",p);
    buf=(char*)((void*)buf+p);

    inode* ino;
    get_inode_by_id(inode_id,&ino);
    if(start>=ino->size){
        return -1;
    }
    char* addr=(char*)(((void*)FILE_DATA_ADDR)+mul(ino->start_block,BLOCK_SIZE));
    int tmp=1;
    int cnt=0;
    for(int i=0;i<ino->size;i++){
        if(mod(tmp++,4093)==0){
            uint32_t next=*((uint32_t*)addr);
            addr=(char*)(((void*)FILE_DATA_ADDR)+mul(((next<<4)>>4),BLOCK_SIZE));//calc addr of next block;
            i--;
            continue;
        }
        else if(i>=start){
            *buf=*addr;
            buf+=1;
            cnt+=1;
        }
        if(cnt>=count){
            // printk("\n%d,%d",cnt,count);
            break;
        }
        addr+=1;//get data;
    }
    *buf='\0';
    return 0;
}

int writek(uint32_t inode_id,char* buf,uint32_t start,uint32_t count){//the start should not bigger than size
    // print("buf:%s;",buf);
    inode* ino=NULL;
    get_inode_by_id(inode_id,&ino);
    //write data
    char* addr=(char*)(((void*)FILE_DATA_ADDR)+mul(ino->start_block,BLOCK_SIZE));
    ino->size=start+count;
    int tmp=1;
    int cnt=0;
    int i=0;
    //TODO:优化递归操作，每一次写入都会从头开始遍历
    while(1){//递归遍历块
        if(mod(tmp++,4093)==0){
            uint32_t next=*((uint32_t*)addr);
            if(((next<<4)>>4)==0){//没有下一block
                (*(uint32_t*)addr)=next+alloc_block();//增加指向下一block
            }
            next=*((uint32_t*)addr);
            // printk("alloc_inst:%d\n",next);
            addr=(char*)(((void*)FILE_DATA_ADDR)+mul(((next<<4)>>4),BLOCK_SIZE));//calc addr of next block;
            continue;
        }
        else if(i>=start){
            *addr=*buf;buf+=1;
            cnt+=1;
        }
        if(cnt>=count){
            break;
        }
        addr+=1;//get data;
        i+=1;
    }
    for(int j=i;j<i+BLOCK_SIZE;j++){//将剩余未写入块清除
        if(mod(tmp++,4093)==0){
            uint32_t next=*((uint32_t*)addr);
            delete_block_link((next<<4)>>4);
            break;
        }
    }
    
}

int write_i(uint32_t inode_id,char* buf,uint32_t start,uint32_t count){//the start should not bigger than size
    uint32_t p=0;
    __asm__ volatile(
        "csrr %0,0x181"
        :"=r"(p)
    );
    p=(p<<1)>>1;//去除mmu标志位
    buf=(char*)((void*)buf+p);

    // print("buf:%s;",buf);
    inode* ino=NULL;
    get_inode_by_id(inode_id,&ino);
    //write data
    char* addr=(char*)(((void*)FILE_DATA_ADDR)+mul(ino->start_block,BLOCK_SIZE));
    ino->size=start+count;
    int tmp=1;
    int cnt=0;
    int i=0;
    //TODO:优化递归操作，每一次写入都会从头开始遍历
    while(1){//递归遍历块
        if(mod(tmp++,4093)==0){
            uint32_t next=*((uint32_t*)addr);
            if(((next<<4)>>4)==0){//没有下一block
                (*(uint32_t*)addr)=next+alloc_block();//增加指向下一block
            }
            next=*((uint32_t*)addr);
            // printk("alloc_inst:%d\n",next);
            addr=(char*)(((void*)FILE_DATA_ADDR)+mul(((next<<4)>>4),BLOCK_SIZE));//calc addr of next block;
            continue;
        }
        else if(i>=start){
            *addr=*buf;buf+=1;
            cnt+=1;
        }
        if(cnt>=count){
            break;
        }
        addr+=1;//get data;
        i+=1;
    }
    for(int j=i;j<i+BLOCK_SIZE;j++){//将剩余未写入块清除
        if(mod(tmp++,4093)==0){
            uint32_t next=*((uint32_t*)addr);
            delete_block_link((next<<4)>>4);
            break;
        }
    }
    
}

int open_i(char* file_path,uint32_t* inode_id,int* status){
    uint32_t p=0;
    __asm__ volatile(
        "csrr %0,0x181"
        :"=r"(p)
    );
    p=(p<<1)>>1;//去除mmu标志位
    file_path=(char*)((void*)file_path+p);
    inode_id=(uint32_t*)((void*)inode_id+p);
    status=(int*)((void*)status+p);

    //解析地址
    uint32_t file_path_len=str_len(file_path);
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
                printk("path is not exist\n");
                *status=-1;//can not find file;
                return 0;
            }
            stack_ptr=0;
        }else{
            stack[stack_ptr++]=file_path[i];
        }
    }
    inode* ino;
    if(stack_ptr){
        stack[stack_ptr]='\0';
        int in=find_file_in_dir(cur_inode_id,stack);
        if(in!=-1){
            *inode_id=in;
            *status=0;
        }
        else{
            printk("file is not exist\n");
            *status=-1;
            return 0;
        }
    }else if(file_path_len==1){//只有根目录
        get_inode_by_id(0,&ino);
        if(ino->type==0){
            printk("root hasn't been created\n");
            *status=-1;
            return 0;
        }else{
            *inode_id=0;
            *status=0;
        }
    }
    return 0;
}

int create_i(char* file_path,char type,uint32_t* inode_id,int* status){
    uint32_t p=0;
    __asm__ volatile(
        "csrr %0,0x181"
        :"=r"(p)
    );
    p=(p<<1)>>1;//去除mmu标志位
    file_path=(char*)((void*)file_path+p);
    inode_id=(uint32_t*)((void*)inode_id+p);
    status=(int*)((void*)status+p);

    //解析地址
    uint32_t file_path_len=str_len(file_path);
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
                printk("path is not exist\n");
                *status=-1;//can not find file;
                return 0;
            }
            stack_ptr=0;
        }else{
            stack[stack_ptr++]=file_path[i];
        }
    }
    inode* ino;
    if(stack_ptr){//如果/后还有名称,/home/test
        stack[stack_ptr]='\0';
        printk("create_file_name:%s\n",stack);
        int in=find_file_in_dir(cur_inode_id,stack);
        if(in!=-1){
            printk("file has been exist\n");
            *status=-2;//重名文件
            return 0;
        }
        else{
            get_inode_by_id(cur_inode_id,&ino);
            if(ino->type=='d'){
                *status=0;
                *inode_id=(uint32_t)create_inode(stack,type);
                char buf[4];
                uint32_to_char(*inode_id,buf);
                writek(cur_inode_id,buf,ino->size,4);
            }else{
                printk("parent is not a dir\n");
                *status=-3;
                return 0;
            }
        }
    }else if(file_path_len==1){//只有根目录
        get_inode_by_id(0,&ino);
        if(ino->type==0){
            *inode_id=create_inode(file_path,type);
            *status=0;
        }else{
            printk("root has been exist\n");
            *status=-1;//已存在
            return 0;
        }
    }
    return 0;
}

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
