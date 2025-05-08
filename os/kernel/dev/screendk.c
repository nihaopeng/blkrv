#include "graphic.h"

int draw_trianglek(point* p1,point* p2,point* p3,color* c){
    // uint32_t addr=(*((uint32_t*)GPU_ADDR_FREE)==1)?GPU_ADDR_CACHE1:GPU_ADDR_CACHE2;
    uint32_t addr=GPU_ADDR_CACHE;

    while(*(uint32_t*)addr!=0);

    *((uint32_t*)(addr+4))=(uint32_t)p1->x;
    *((uint32_t*)(addr+8))=(uint32_t)p1->y;
    *((uint32_t*)(addr+12))=(uint32_t)p2->x;
    *((uint32_t*)(addr+16))=(uint32_t)p2->y;
    *((uint32_t*)(addr+20))=(uint32_t)p3->x;
    *((uint32_t*)(addr+24))=(uint32_t)p3->y;

    *((uint32_t*)(addr+28))=(uint32_t)c->red;
    *((uint32_t*)(addr+32))=(uint32_t)c->green;
    *((uint32_t*)(addr+36))=(uint32_t)c->blue;
    
    *((uint32_t*)addr)=2;//2 represents draw a triangle
    return 0;
}

int flushk(){
    // uint32_t addr=(*((uint32_t*)GPU_ADDR_FREE)==1)?GPU_ADDR_CACHE1:GPU_ADDR_CACHE2;
    uint32_t addr=GPU_ADDR_CACHE;

    while(*(uint32_t*)addr!=0);

    *((uint32_t*)addr)=3;//3 represents flush operation
}

int draw_labelk(point* p,char* str,color* c,int font){
    // uint32_t addr=(*((uint32_t*)GPU_ADDR_FREE)==1)?GPU_ADDR_CACHE1:GPU_ADDR_CACHE2;
    uint32_t addr=GPU_ADDR_CACHE;

    while(*(uint32_t*)addr!=0);

    *((uint32_t*)(addr+4))=(uint32_t)(p->x);
    *((uint32_t*)(addr+8))=(uint32_t)(p->y);
    *((uint32_t*)(addr+12))=(uint32_t)(c->red);
    *((uint32_t*)(addr+16))=(uint32_t)(c->green);
    *((uint32_t*)(addr+20))=(uint32_t)(c->blue);
    *((uint32_t*)(addr+24))=(uint32_t)font;

    for(int i=0;;i++){
        if(*(str+i)!=0)
            *((char*)(addr+28+i))=*(str+i);
        else
            break;
    }

    *((uint32_t*)addr)=1;//1 represents draw text
}

uint32_t get_next_block_id_s(uint32_t cur_block_id){
    return *((uint32_t*)FAT_START+cur_block_id);
}

screen_inode* get_inode_by_id_s(uint32_t inode_id){
    return ((screen_inode*)INODE_START)+inode_id;
}

uint32_t* get_block_addr_s(uint32_t block_id){
    return (uint32_t*)(DATA_START+block_id*BLOCK_SIZE);
}

// 读取文件
int readk_s(uint32_t inode_id, char* buf, uint32_t start, uint32_t count) {
    screen_inode* f_inode=get_inode_by_id_s(inode_id);
    uint32_t block_id=f_inode->start_block;
    for(int i=0;i<(start/BLOCK_SIZE);i++){
        block_id=get_next_block_id_s(block_id);
        if(block_id==EOF) return 0;
    }
    char* block_addr=(char*)get_block_addr_s(block_id);
    char* fp=block_addr+start%BLOCK_SIZE;
    uint32_t cur_size=(start/BLOCK_SIZE)*BLOCK_SIZE+start%BLOCK_SIZE;
    uint32_t pos=0;
    // printk("read:block_id:%d,block_addr:%x,fp:%x\n",block_id,block_addr,fp);
    for(int i=0;i<count;i++){
        if(cur_size>=f_inode->size){
            return pos;
        }
        buf[pos++]=*fp;
        cur_size+=1;
        fp+=1;
        if(((uint32_t)fp-(uint32_t)DATA_START)%(uint32_t)BLOCK_SIZE==0){
            block_id=get_next_block_id_s(block_id);
            if(block_id==EOF) return pos;
            fp=(char*)get_block_addr_s(block_id);
        }
    }
    return pos;
}

int draw_jpgk(uint32_t inode_id,uint32_t size,uint32_t x,uint32_t y){
    // uint32_t addr=(*((uint32_t*)GPU_ADDR_FREE)==1)?GPU_ADDR_CACHE1:GPU_ADDR_CACHE2;
    uint32_t addr=GPU_ADDR_CACHE;

    while(*(uint32_t*)addr!=0);
    printk("x:%d,y:%d,size:%d\n",x,y,size);
    *((uint32_t*)(addr+4))=(uint32_t)(x);
    *((uint32_t*)(addr+8))=(uint32_t)(y);
    *((uint32_t*)(addr+12))=(uint32_t)(size);
    char buf[512];
    uint32_t fp=0;
    while(size){
        printk("size:%d\n",size);
        if(size>=512){
            readk_s(inode_id,buf,fp,512);
            for(int i=0;i<512;i++){
                *((char*)(addr+16+fp+i))=buf[i];
            }
            fp+=512;
            size-=512;
        }else{
            readk_s(inode_id,buf,fp,size);
            for(int i=0;i<size;i++){
                *((char*)(addr+16+fp+i))=buf[i];
            }
            fp+=size;
            size-=size;
        }
    }
    printk("start show jpg\n");
    *((uint32_t*)addr)=4;//4 represents draw jpg
}