#include "graphic.h"

int draw_triangle_i(point* p1,point* p2,point* p3,color* c){
    _vir2phyk(point*,p1);
    _vir2phyk(point*,p2);
    _vir2phyk(point*,p3);
    _vir2phyk(color*,c);

    draw_trianglek(p1,p2,p3,c);
}

int flush_i(){
    flushk();
}

int draw_label_i(point* p1,char* str,color* c,int font){
    _vir2phyk(point*,p1);
    _vir2phyk(char*,str);
    _vir2phyk(color*,c);

    draw_labelk(p1,str,c,font);
}

int draw_jpg_i(uint32_t inode_id,uint32_t size,uint32_t x,uint32_t y){
    return draw_jpgk(inode_id,size,x,y);
}

_regist_syscall(void,draw_jpg);

_regist_syscall(void,draw_triangle);

_regist_syscall(void,draw_label);

_regist_syscall(void,flush);
