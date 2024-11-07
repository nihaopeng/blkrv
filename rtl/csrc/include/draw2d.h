#ifndef _DRAW2D_H_
#define _DRAW2D_H_
#include "tgaimage.h"
//#include "model.h"
#include "geometry.h"
#include <conio.h>

// struct Color
// {
//     TGAColor white=TGAColor(255,255,255,255);
//     TGAColor black=TGAColor(0,0,0,255);
//     TGAColor green=TGAColor(0,255,0,255);
//     TGAColor blue=TGAColor(255,0,0,255);
//     TGAColor red=TGAColor(0,0,255,255);
//     TGAColor purple=TGAColor(240,32,160,255);
//     TGAColor pink=TGAColor(203,192,255,255);
// };

class Draw2d
{
    public:
    TGAImage* image=new TGAImage(1024,1024,4);
    Draw2d()=default;
    void line(int x0,int y0,int x1,int y1,const TGAColor& color);
    void circle(int cx,int cy,int radius,const TGAColor& color);
    void fillcolor(vec2 v0,vec2 v1,vec2 v2,const TGAColor& color);
    void fillcolor(int cx,int cy,int radius,const TGAColor& color);
    void character(std::string character,int x,int y,int size,const TGAColor& color);
};

#endif