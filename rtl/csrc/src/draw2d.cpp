#include "tgaimage.h"
#include "draw2d.h"

void Draw2d::line(int x0,int y0,int x1,int y1,const TGAColor& color)
{
    if(x0>x1)
    {
        int temp;
        temp=x0;x0=x1;x1=temp;
        temp=y0;y0=y1;y1=temp;
    }
    //flag1:slope>1   flag2:slope<0且翻转后slope>1   
    //flag3:slpoe<0且翻转后0<slpope<1
    int flag1=0,flag2=0,flag3=0;
    if(y1-y0>x1-x0)//slope>1
    {
        int temp;
        temp=y0;y0=x0;x0=temp;
        temp=y1;y1=x1;x1=temp;
        flag1=1;
    }
    else if(y1<y0)//slope<0
    {
        y0=image->height()-y0;y1=image->height()-y1;
        if(y1-y0>x1-x0)
        {
            int temp;
            temp=y0;y0=x0;x0=temp;
            temp=y1;y1=x1;x1=temp;
            flag2=1;
        }
        else flag3=1;
    }
    int dx=x1-x0,dy=y1-y0;
    int incrE=2*dy,incrNE=2*(dy-dx);
    int d=2*dy-dx;
    int x=x0,y=y0;
    if(flag1) image->set(y0,x0,color);
    else if(flag2) image->set(y0,image->height()-x0,color);
    else if(flag3) image->set(x0,image->height()-y0,color);
    else image->set(x0,y0,color);
    while (x<x1)
    {
        x++;
        if(d<0) d+=incrE;//下一个像素点在东方
        else//下一个像素点在东北方
        {
            d+=incrNE;
            y++;
        }
        if(flag1) image->set(y,x,color);
        else if(flag2) image->set(y,image->height()-x,color);
        else if(flag3) image->set(x,image->height()-y,color);
        else image->set(x,y,color);
    }
}

void Draw2d::circle(int cx,int cy,int radius,const TGAColor& color)
{
    int x=0,y=radius;//先按照圆心在原点计算，再平移
    int d=1-radius;//d本应该初始等于1.25-radius，此处d=d-0.25方便计算
    while(y>=x)//八分之一圆
    {
        image->set(x+cx,y+cy,color);
        image->set(x+cx,cy-y,color);
        image->set(cx-x,cy-y,color);
        image->set(cx-x,y+cy,color);
        image->set(y+cx,x+cy,color);
        image->set(y+cx,cy-x,color);
        image->set(cx-y,cy-x,color);
        image->set(cx-y,x+cy,color);
        if(d<=0) //下一个像素点在东方
        {
            x++;
            d=d+2*x+3;
        }
        else //下一个像素点在东南方
        {
            x++;
            y--;
            d=d+2*(x-y)+5;
        }
    }
}

void Draw2d::fillcolor(vec2 v0,vec2 v1,vec2 v2,const TGAColor& color)
{
    if(v0.y==v1.y && v0.y==v2.y) return;
    if(v0.y>v1.y) std::swap(v0,v1);
    if(v0.y>v2.y) std::swap(v0,v2);
    if(v1.y>v2.y) std::swap(v1,v2);
    int vertical_height=v2.y-v0.y;
    for(int i=0;i<vertical_height;i++)
    {
        bool half=i>v1.y-v0.y || v1.y==v0.y;
        int height=half?v2.y-v1.y:v1.y-v0.y;
        float a=(float)i/vertical_height;
        float b=half?(float)(i-v1.y+v0.y)/height:(float)i/height;
        int x0=a*(v2.x-v0.x)+v0.x;
        int x1=half?b*(v2.x-v1.x)+v1.x:b*(v1.x-v0.x)+v0.x;
        line(x0,v0.y+i,x1,v0.y+i,color);
    }
}

void Draw2d::fillcolor(int cx,int cy,int radius,const TGAColor& color)
{
    for(int i=0;i<radius;i++)
        circle(cx,cy,i,color);
}

void Draw2d::character(std::string character,int x,int y,int size,const TGAColor& color)
{
    
}