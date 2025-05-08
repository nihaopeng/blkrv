#include "ini.h"
#include "set_gate.h"
#include "syscall.h"
#include "drivers.h"

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

typedef struct screen_file{
    char file_name[MAX_NAME];
    uint32_t size;
    uint32_t start_block;
    uint8_t type;
} screen_inode;

int draw_triangle_i(point* p1,point* p2,point* p3,color* col);

int draw_trianglek(point* p1,point* p2,point* p3,color* col);

int draw_triangle(point* p1,point* p2,point* p3,color* col);

int flush_i();

int flushk();

int flush();

int draw_label_i(point* p,char* str,color* c,int font);

int draw_labelk(point* p,char* str,color* c,int font);

int draw_label(point* p,char* str,color* c,int font);

int draw_jpg_i(uint32_t inode_id,uint32_t size,uint32_t x,uint32_t y);

int draw_jpgk(uint32_t inode_id,uint32_t size,uint32_t x,uint32_t y);

int draw_jpg(uint32_t inode_id,uint32_t size,uint32_t x,uint32_t y);

void regist_draw_jpg(int* dt_addr);

void regist_draw_triangle(int* dt_addr);

void regist_draw_label(int* dt_addr);

void regist_flush(int* dt_addr);