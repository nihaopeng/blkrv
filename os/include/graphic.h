#include "ini.h"
#include "set_gate.h"
#include "syscall.h"

typedef struct point
{
    int x;
    int y;
}point;

typedef struct color
{
    int red;
    int green;
    int blue;
    int alpha;
    /* data */
}color;

int draw_triangle_i(point* p1,point* p2,point* p3,color* col);

int draw_trianglek(point* p1,point* p2,point* p3,color* col);

int draw_triangle(point* p1,point* p2,point* p3,color* col);

int flush_i();

int flushk();

int flush();

int draw_label_i(point* p,char* str,color* c,int font);

int draw_labelk(point* p,char* str,color* c,int font);

int draw_label(point* p,char* str,color* c,int font);

void regist_draw_triangle(int* dt_addr);

void regist_draw_label(int* dt_addr);

void regist_flush(int* dt_addr);