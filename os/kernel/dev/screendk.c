#include "graphic.h"

int cur_cache=1;

void draw_trianglek(point* p1,point* p2,point* p3,color* c){
    uint32_t addr=(cur_cache==1)?GPU_ADDR_CACHE1:GPU_ADDR_CACHE2;
    cur_cache=(cur_cache==1)?2:1;

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
    return;
}

void flushk(){
    uint32_t addr=(cur_cache==1)?GPU_ADDR_CACHE1:GPU_ADDR_CACHE2;
    cur_cache=(cur_cache==1)?2:1;

    while(*(uint32_t*)addr!=0);

    *((uint32_t*)addr)=3;//2 represents flush operation
}

void draw_labelk(point* p,char* str,color* c,int font){
    uint32_t addr=(cur_cache==1)?GPU_ADDR_CACHE1:GPU_ADDR_CACHE2;
    cur_cache=(cur_cache==1)?2:1;

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

    *((uint32_t*)addr)=1;//2 represents draw text
}