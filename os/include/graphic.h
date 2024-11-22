#include "ini.h"

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

void draw_triangle_i(point* p1,point* p2,point* p3,color* col);

void draw_trianglek(point* p1,point* p2,point* p3,color* col);

void flush_i();

void flushk();

void draw_label_i(point* p,char* str,color* c,int font);

void draw_labelk(point* p,char* str,color* c,int font);