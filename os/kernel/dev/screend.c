#include "graphic.h"

void draw_triangle_i(point* p1,point* p2,point* p3,color* c){
    uint32_t p=0;
    __asm__ volatile(
        "csrr %0,0x181"
        :"=r"(p)
    );
    p=(p<<1)>>1;//去除mmu标志位
    p1=(point*)((void*)p1+p);
    p2=(point*)((void*)p2+p);
    p3=(point*)((void*)p3+p);
    c=(color*)((void*)c+p);

    draw_trianglek(p1,p2,p3,c);
}

void flush_i(){
    flushk();
}

void draw_label_i(point* p1,char* str,color* c,int font){
    uint32_t p=0;
    __asm__ volatile(
        "csrr %0,0x181"
        :"=r"(p)
    );
    p=(p<<1)>>1;//去除mmu标志位
    p1=(point*)((void*)p1+p);
    str=(char*)((void*)str+p);
    c=(color*)((void*)c+p);

    draw_labelk(p1,str,c,font);
}