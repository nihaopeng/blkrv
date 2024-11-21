#include "file.h"

int init_fs(){
    uint32_t inode_id;
    int status;
    createk("/",'d',&inode_id,&status);
    createk("/include",'d',&inode_id,&status);
    createk("/tmp",'d',&inode_id,&status);
    createk("/tmp/test.bin",'f',&inode_id,&status);
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
            // printk("inode:%s;",ino->file_name);
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
    inode* ino=NULL;
    get_inode_by_id(inode_id,&ino);
    if(start>ino->size)
        return -1;
    //write data
    char* addr=(char*)(((void*)FILE_DATA_ADDR)+mul(ino->start_block,BLOCK_SIZE));
    uint32_t global_offset=0;
    uint32_t global_cnt=0;
    while(start>4092){
        addr+=4092;
        int next=(*(int*)addr)&0x0fffffff;//去除前四位
        addr=(char*)(((void*)FILE_DATA_ADDR)+mul(next,BLOCK_SIZE));
        start-=4092;
        global_cnt+=4092;
    }
    addr+=start;
    global_offset+=start;
    // printk("size:%d,count:%d",ino->size,count);
    for(int i=0;i<count&&global_cnt<ino->size;i++,global_cnt++){
        if(global_offset==4092){
            int next=(*(int*)addr)&0x0fffffff;//去除前四位
            // printk("next:%d\n",next);
            addr=(char*)(((void*)FILE_DATA_ADDR)+mul(next,BLOCK_SIZE));
            global_offset=0;
            i-=1;
            global_cnt-=1;
        }else{
            global_offset+=1;
            buf[i]=*addr;
            addr+=1;
        }
        // printk("addr:%d\n",addr);
    }
    // printk("read_data:%s\n",buf);
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

    inode* ino=NULL;
    get_inode_by_id(inode_id,&ino);
    if(start>ino->size)
        return -1;
    //write data
    char* addr=(char*)(((void*)FILE_DATA_ADDR)+mul(ino->start_block,BLOCK_SIZE));
    ino->size=start+count;
    uint32_t global_offset=0;
    uint32_t global_cnt=0;
    while(start>4092){
        addr+=4092;
        int next=(*(int*)addr)&0x0fffffff;//去除前四位
        addr=(char*)(((void*)FILE_DATA_ADDR)+mul(next,BLOCK_SIZE));
        start-=4092;
        global_cnt+=4092;
    }
    addr+=start;
    global_offset+=start;
    for(int i=0;i<count,global_cnt<ino->size;i++,global_cnt++){
        if(global_offset==4092){
            int next=(*(int*)addr)&0x0fffffff;//去除前四位
            addr=(char*)(((void*)FILE_DATA_ADDR)+mul(next,BLOCK_SIZE));
            global_offset=0;
            i-=1;
            global_cnt-=1;
        }else{
            global_offset+=1;
            buf[i]=*addr;
            addr+=1;
        }
    }
}

int writek(uint32_t inode_id,char* buf,uint32_t start,uint32_t count){//the start should not bigger than size
    inode* ino=NULL;
    get_inode_by_id(inode_id,&ino);
    if(start>ino->size){
        printk("start out of file size\n");
        return 0;
    }
    //write data
    char* addr=(char*)(((void*)FILE_DATA_ADDR)+mul(ino->start_block,BLOCK_SIZE));
    ino->size=start+count;
    uint32_t global_offset=0;
    while(start>4092){
        addr+=4092;
        int next=(*(int*)addr)&0x0fffffff;//去除前四位
        // printk("recur_next:%d\n",next);
        addr=(char*)(((void*)FILE_DATA_ADDR)+mul(next,BLOCK_SIZE));
        start-=4092;
    }
    addr+=start;
    global_offset+=start;
    for(int i=0;i<count;i++){
        if(global_offset==4092){
            int next=(*(int*)addr)&0x0fffffff;//去除前四位
            if(next==0){
                (*(int*)addr)=(*(int*)addr)+alloc_block();
            }
            next=(*(int*)addr)&0x0fffffff;//去除前四位
            // printk("next:%d\n",next);
            addr=(char*)(((void*)FILE_DATA_ADDR)+mul(next,BLOCK_SIZE));
            global_offset=0;
            i-=1;
        }else{
            global_offset+=1;
            *addr=buf[i];
            addr+=1;
        }
    }
    addr=addr+4092-global_offset;
    (*(uint32_t*)addr)=(*(uint32_t*)addr)&0xf0000000;
    int next=(*(int*)addr)&0x0fffffff;
    delete_block_link(next);//TODO:未测试
}

int write_i(uint32_t inode_id,char* buf,uint32_t start,uint32_t count){//the start should not bigger than size
    uint32_t p=0;
    __asm__ volatile(
        "csrr %0,0x181"
        :"=r"(p)
    );
    p=(p<<1)>>1;//去除mmu标志位
    buf=(char*)((void*)buf+p);

    inode* ino=NULL;
    get_inode_by_id(inode_id,&ino);
    if(start>ino->size)
        return 0;
    //write data
    char* addr=(char*)(((void*)FILE_DATA_ADDR)+mul(ino->start_block,BLOCK_SIZE));
    ino->size=start+count;
    uint32_t global_offset=0;
    while(start>4092){
        addr+=4092;
        int next=(*(int*)addr)&0x0fffffff;//去除前四位
        addr=(char*)(((void*)FILE_DATA_ADDR)+mul(next,BLOCK_SIZE));
        start-=4092;
    }
    addr+=start;
    global_offset+=start;
    for(int i=0;i<count;i++){
        if(global_offset==4092){
            int next=(*(int*)addr)&0x0fffffff;//去除前四位
            if(next==0){
                (*(uint32_t*)addr)=(*(int*)addr)+alloc_block();
            }
            next=(*(int*)addr)&0x0fffffff;//去除前四位
            addr=(char*)(((void*)FILE_DATA_ADDR)+mul(next,BLOCK_SIZE));
            global_offset=0;
            i-=1;
        }else{
            global_offset+=1;
            *addr=buf[i];
            addr+=1;
        }
    }
    addr=addr+4092-global_offset;
    (*(uint32_t*)addr)=(*(uint32_t*)addr)&0xf0000000;
    int next=(*(int*)addr)&0x0fffffff;
    delete_block_link(next);//TODO:未测试
}

int openk(char* file_path,uint32_t* inode_id,int* status){
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

int createk(char* file_path,char type,uint32_t* inode_id,int* status){
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
                // printk("inode_id:%s\n",buf);
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
