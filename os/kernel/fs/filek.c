#include "file.h"

uint32_t get_next_block_id(uint32_t cur_block_id){
    return *((uint32_t*)FAT_START+cur_block_id);
}

uint32_t set_next_block_id(uint32_t cur_block_id,uint32_t next_block_id){
    *((uint32_t*)FAT_START+cur_block_id)=next_block_id;
    return 0;
}

uint32_t* get_block_addr(uint32_t block_id){
    return (uint32_t*)(DATA_START+block_id*BLOCK_SIZE);
}

int alloc_block(){
    int j=0;
    for(uint32_t* i=(uint32_t*)FAT_START;i<(uint32_t*)DATA_START;i++,j++){
        if(*i==0){
            *i=EOF;
            return j;
        }
    }
    return -1;
}

int free_block(uint32_t block_id){//递归释放块
    // printk("free block recursive:%d\n",block_id);
    if(block_id==EOF){
        return 0;
    }
    free_block(get_next_block_id(block_id));
    *((uint32_t*)FAT_START+block_id)=0;
    return 0;
}

inode* get_inode_by_id(uint32_t inode_id){
    return ((inode*)INODE_START)+inode_id;
}

inode* get_inode_in_dir_by_name(char* file_name,inode* dir_inode){
    uint32_t block_id=dir_inode->start_block;
    uint32_t* block_addr=get_block_addr(block_id);
    while(block_id!=EOF){//遍历所有块
        for(uint32_t j=0;j<(BLOCK_SIZE>>2);j++){
            uint32_t inode_id=*block_addr;
            if(inode_id!=0){//空闲位跳过, 但仍推进槽位
                inode* f_inode=get_inode_by_id(inode_id);
                if(str_cmp(f_inode->file_name,file_name)){//文件名相等，找到文件
                    return f_inode;
                }
            }
            block_addr+=1;
        }
        uint32_t next_block_id=get_next_block_id(block_id);
        block_id=next_block_id;
        block_addr=get_block_addr(block_id);
    }
    return NULL;
}

int add_inode_id_to_dir(inode* dir_inode,uint32_t file_id){
    uint32_t block_id=dir_inode->start_block;
    uint32_t prev_block_id=block_id;
    uint32_t* block_addr=get_block_addr(block_id);
    uint32_t pos=0;//槽位序号
    while(block_id!=EOF){//遍历所有块
        for(uint32_t j=0;j<(BLOCK_SIZE>>2);j++){//遍历一个块内的所有数据
            uint32_t inode_id=*block_addr;
            if(inode_id==file_id){//去重: 已存在则不重复添加
                return 1;
            }
            if(inode_id==0){//空闲位: 直接复用, size 维护为"最高占用槽+1"
                *block_addr=file_id;
                if(pos*4>=dir_inode->size) dir_inode->size=pos*4+4;
                return 1;
            }
            block_addr+=1;
            pos++;
        }
        uint32_t next_block_id=get_next_block_id(block_id);
        prev_block_id=block_id;
        block_id=next_block_id;
        block_addr=get_block_addr(block_id);
    }
    //文件分配的块用完了，分配新的块
    uint32_t new_block_id=alloc_block();
    if(new_block_id==-1) return NULL;
    if(dir_inode->start_block==(uint32_t)EOF){
        dir_inode->start_block=new_block_id;
    }
    else{
        set_next_block_id(prev_block_id,new_block_id);//更新fat表
    } 
    uint32_t* data_block=get_block_addr(new_block_id);
    *data_block=file_id;
    if(pos*4>=dir_inode->size) dir_inode->size=pos*4+4;
    return 1;
}

int delete_inode(uint32_t inode_id){
    inode* finode=(inode*)INODE_START+inode_id;
    str_cpy("",finode->file_name);
    finode->type=0;
    finode->size=0;
    finode->start_block=0;
}

// 在所有目录的块中查找并移除 inode_id 条目 (删除文件后目录不应残留幽灵条目)
static int remove_dir_entry(uint32_t inode_id){
    // 扫描上限 1024: 覆盖本文件系统实际 inode 数 (性能考虑; 正确性由调度器嵌套中断修复保证,
    // 与扫描长度无关)
    for(uint32_t i=ROOT_INODE_ID;i<1024;i++){
        inode* d=(inode*)INODE_START+i;
        if(d->type!=DIR_TYPE) continue;
        uint32_t block_id=d->start_block;
        uint32_t slot=0;//全局槽位序号(跨块累计)
        int removed=0;
        while(block_id!=EOF){
            uint32_t* p=(uint32_t*)get_block_addr(block_id);
            for(uint32_t j=0;j<(BLOCK_SIZE>>2);j++){
                if(p[j]==inode_id){
                    p[j]=0;
                    removed=1;
                }
                slot++;
            }
            block_id=get_next_block_id(block_id);
        }
        if(removed){
            // 目录 size 维护为 "最高占用槽+1" 的字节数; 全部清空则为 0,
            // 这样删除子项后空目录本身才能被 deletek 删除
            uint32_t high=0;
            block_id=d->start_block;
            uint32_t s=0;
            while(block_id!=EOF){
                uint32_t* p=(uint32_t*)get_block_addr(block_id);
                for(uint32_t j=0;j<(BLOCK_SIZE>>2);j++){
                    if(p[j]!=0) high=s+1;
                    s++;
                }
                block_id=get_next_block_id(block_id);
            }
            d->size=high*4;
            return 1;
        }
    }
    return 0;
}

int deletek(uint32_t inode_id){
    inode* finode=(inode*)INODE_START+inode_id;
    if(finode->type==DIR_TYPE&&finode->size>0){// 非空目录拒绝删除, 避免子项孤儿化
        return -1;
    }
    remove_dir_entry(inode_id);// 从父目录移除条目
    free_block(finode->start_block);
    delete_inode(inode_id);
    return 1;
}

uint32_t create_inode(char* file_name,char type){
    uint32_t j=ROOT_INODE_ID;
    for(inode* i=(inode*)INODE_START+ROOT_INODE_ID;i<(inode*)FAT_START;i++,j++){//遍历inode
        if(i->type==0){//找到inode空闲位
            // 不在此清理旧目录项: add_inode_id_to_dir 的去重逻辑会复用旧的目录项 id
            str_cpy(file_name,i->file_name);
            i->type=type;
            i->start_block=EOF;
            i->size=0;
            return j;
        }
    }
    return NULL;
}

uint32_t create_file(char* file_name,char type,inode* parent_inode){
    uint32_t inode_id=create_inode(file_name,type);
    add_inode_id_to_dir(parent_inode,inode_id);
    return inode_id;
}

uint32_t create_Recursive(char* file_path, char type, uint32_t* inode_id,uint32_t depth){
    //获取最后一个'/'的地址
    char* last_seperator=strrchr(file_path,'/');
    *last_seperator=0;//拆分父目录和文件，/include/tmp.h->/include\0tmp.h->\0include
    char* parent=file_path;
    char* filename=last_seperator+1;
    uint32_t parent_inode_id=0;
    // printk("depth:%d,parent_path:%s,filename:%s\n",depth,parent,filename);
    if(file_path==last_seperator){//此时父文件夹为根目录
        *last_seperator='/';//!!!恢复静态字符串，编译时相同字符会被编译为相同地址。
        inode* root_inode=get_inode_by_id(ROOT_INODE_ID);
        if(root_inode->type==0){//根目录不存在
            // printk("root not exist\n");
            str_cpy("/\0",root_inode->file_name);
            root_inode->type=DIR_TYPE;
            root_inode->start_block=EOF;
            root_inode->size=0;
        }
        if(str_len(filename)==0){
            return ROOT_INODE_ID;
        }
        inode* file_inode=get_inode_in_dir_by_name(filename,root_inode);
        if(!file_inode){//如果文件不存在于根目录中
            // printk("file:%s not exist\n",filename);
            //创建文件
            if(depth==0){
                return create_file(filename,type,root_inode);
            }else{
                return create_file(filename,DIR_TYPE,root_inode);
            }
        }else{
            // printk("file:%s exist in /\n",filename);
            return (file_inode-(inode*)INODE_START);//返回inode-id
        }
    }else{
        parent_inode_id=create_Recursive(parent,type,inode_id,depth+1);
    }
    //创建本层文件。
    *last_seperator='/';
    // printk("parent_inode_id:%d,depth:%d\n",parent_inode_id,depth);
    if(str_len(filename)==0){
        return parent_inode_id;
    }
    inode* file_inode=get_inode_in_dir_by_name(filename,get_inode_by_id(parent_inode_id));
    if(!file_inode){//文件不存在
        // printk("file:%s not exist\n",filename);
        if(depth==0){
            return create_file(filename,type,get_inode_by_id(parent_inode_id));
        }else{
            return create_file(filename,DIR_TYPE,get_inode_by_id(parent_inode_id));
        }
    }else{
        return (file_inode-(inode*)INODE_START);//返回inode-id
    }
}

// 创建文件或目录（支持自动创建父目录）
int createk(char* file_path, char type, uint32_t* inode_id) {
    return create_Recursive(file_path,type,inode_id,0);
}

// 打开文件（返回inode_id）
int openk(const char* file_path) {
    //获取最后一个'/'的地址
    // printk("file_path:%s\n",file_path);
    char* last_seperator=strrchr(file_path,'/');
    *last_seperator=0;//拆分父目录和文件，/include/tmp.h->/include\0tmp.h->\0include
    char* parent=file_path;
    char* filename=last_seperator+1;
    uint32_t parent_inode_id=0;
    // printk("file_path:%s,sep:%s\n",file_path,filename);
    if(file_path==last_seperator){//此时父文件夹为根目录
        // printk("it is root\n");
        *last_seperator='/';
        inode* root_inode=get_inode_by_id(ROOT_INODE_ID);
        if(str_len(filename)==0){
            return ROOT_INODE_ID;
        }
        inode* file_inode=get_inode_in_dir_by_name(filename,root_inode);
        if(!file_inode){//如果文件不存在于根目录中
            return -1;
        }else{
            return (file_inode-(inode*)INODE_START);//返回inode-id
        }
    }else{
        parent_inode_id=openk(parent);
    }
    *last_seperator='/';
    if(parent_inode_id==-1){//父文件夹不存在，直接返回。
        return -1;
    }
    if(str_len(filename)==0){
        return parent_inode_id;
    }
    inode* file_inode=get_inode_in_dir_by_name(filename,get_inode_by_id(parent_inode_id));
    if(!file_inode){//文件不存在
        return -1;
    }else{
        return (file_inode-(inode*)INODE_START);//返回inode-id
    }
}

// 读取文件
int readk(uint32_t inode_id, char* buf, uint32_t start, uint32_t count) {
    inode* f_inode=get_inode_by_id(inode_id);
    uint32_t block_id=f_inode->start_block;
    for(int i=0;i<(start/BLOCK_SIZE);i++){
        block_id=get_next_block_id(block_id);
        if(block_id==EOF) return 0;
    }
    char* fp=(char*)get_block_addr(block_id)+(start%BLOCK_SIZE);
    uint32_t cur=start;
    uint32_t pos=0;
    // 用剩余计数代替逐字节取模; 对齐段按字读, 减少 4 倍访存/循环开销
    uint32_t left_in_block=BLOCK_SIZE-(start%BLOCK_SIZE);
    // 前缀: 对齐到 4 字节
    while(pos<count&&cur<f_inode->size&&((((uint32_t)fp&3)!=0)||(((uint32_t)buf+pos)&3)!=0||left_in_block<4)){
        buf[pos++]=*fp++;
        cur++;
        if(--left_in_block==0){
            block_id=get_next_block_id(block_id);
            if(block_id==EOF) return pos;
            fp=(char*)get_block_addr(block_id);
            left_in_block=BLOCK_SIZE;
        }
    }
    // 整字读
    while(pos+4<=count&&cur+4<=f_inode->size&&left_in_block>=4){
        *(uint32_t*)(buf+pos)=*(uint32_t*)fp;
        fp+=4; cur+=4; pos+=4; left_in_block-=4;
        if(left_in_block==0){
            block_id=get_next_block_id(block_id);
            if(block_id==EOF) return pos;
            fp=(char*)get_block_addr(block_id);
            left_in_block=BLOCK_SIZE;
        }
    }
    // 尾部
    while(pos<count&&cur<f_inode->size){
        buf[pos++]=*fp++;
        cur++;
        if(--left_in_block==0){
            block_id=get_next_block_id(block_id);
            if(block_id==EOF) return pos;
            fp=(char*)get_block_addr(block_id);
            left_in_block=BLOCK_SIZE;
        }
    }
    return pos;
}

// 写入文件
int writek(uint32_t inode_id, char* buf, uint32_t start, uint32_t count) {
    inode* f_inode=get_inode_by_id(inode_id);
    uint32_t block_id=f_inode->start_block;
    if(block_id==EOF){//文件大小为0
        block_id=alloc_block();
        f_inode->start_block=block_id;
    }
    for(int i=0;i<(start/BLOCK_SIZE);i++){
        block_id=get_next_block_id(block_id);
        if(block_id==EOF) return 0;//start>file_size
    }
    char* fp=(char*)get_block_addr(block_id)+(start%BLOCK_SIZE);
    uint32_t pos=0;
    uint32_t cur=start;
    uint32_t left_in_block=BLOCK_SIZE-(start%BLOCK_SIZE);
    while(pos<count){
        *fp=buf[pos++];
        cur++;
        if(cur>f_inode->size)
            f_inode->size+=1;
        fp++;
        left_in_block--;
        if(left_in_block==0){
            uint32_t prev_inode_id=block_id;
            block_id=get_next_block_id(block_id);
            if(block_id==(uint32_t)EOF){
                block_id=alloc_block();
                // printk("prev_id:%d,cur_id:%d\n",prev_inode_id,block_id);
                set_next_block_id(prev_inode_id,block_id);
            }
            fp=(char*)get_block_addr(block_id);
            left_in_block=BLOCK_SIZE;
        }
    }
    free_block(get_next_block_id(block_id));//递归释放剩余块
    *((uint32_t*)FAT_START+block_id)=EOF;
    f_inode->size=start+count;
    return 1;
}

// 获取文件信息
int finfo_k(uint32_t inode_id,inode* finode){
    inode* tmp_inode=get_inode_by_id(inode_id);
    str_cpy(tmp_inode->file_name,finode->file_name);
    finode->size=tmp_inode->size;
    finode->start_block=tmp_inode->start_block;
    finode->type=tmp_inode->type;
    return 1;
}

int init_fs(){
    uint32_t root_id;
    printk("create /,inode_id:%d\n",createk("/",DIR_TYPE,&root_id));
    printk("create /include,inode_id:%d\n",createk("/include",DIR_TYPE,&root_id));
    printk("create /tmp,inode_id:%d\n",createk("/tmp",DIR_TYPE,&root_id));
    printk("create /bin,inode_id:%d\n",createk("/bin",DIR_TYPE,&root_id));
    // printk("create /tmp/test.bin,inode_id:%d\n",createk("/tmp/test.bin",FILE_TYPE,&root_id));
    // printk("create /tmp/test/test/test.bin,inode_id:%d\n",createk("/tmp/test/test/test.bin",FILE_TYPE,&root_id));
    // printk("create /tmp/test/test.bin,inode_id:%d\n",createk("/tmp/test/test.bin",FILE_TYPE,&root_id));
    // printk("open /,inode_id:%d\n",openk("/tmp/test/test/test.bin"));
    //below is test
    // uint32_t testbin_inode=openk("/tmp/test/test.bin");
    // char ctx[20]="hello world ";
    // uint32_t ctx_len=str_len(ctx);
    // uint32_t sum1=0;
    // for(int i=0;i<300;i++){
    //     writek(testbin_inode,ctx,i*ctx_len,ctx_len);
    //     sum1+=ctx_len;
    // }
    // char buf[512];
    // uint32_t sum2=0;
    // for(int i=0;i<1000;i++){
    //     uint32_t len=readk(testbin_inode,buf,i*511,511);
    //     buf[511]='\0';
    //     printk("len:%d\n",len);
    //     sum2+=len;
    //     if(len==0){
    //         break;
    //     }
    //     printk("buf:%s\n",buf);
    // }
    // printk("sum1:%d,sum2:%d\n",sum1,sum2);
}
