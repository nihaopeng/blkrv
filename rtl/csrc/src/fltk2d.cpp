#include "fltk2d.h"

my_window::my_window(int W, int H, const char *title):Fl_Double_Window(W,H,title)
{
    this->triangle=new triangle_c(W,H);
    this->clearw=new clear_widget_c(W,H);
    this->text=new text_c(W,H);
}

triangle_c::triangle_c(int w,int h):
Fl_Widget(0,0,w,h)
{
    
}

void triangle_c::draw()
{
    fl_color(this->r,this->g,this->b);
    fl_begin_polygon();
    fl_vertex(x0,y0);
    fl_vertex(x1,y1);
    fl_vertex(x2,y2);
    fl_end_polygon();
}

void triangle_c::add(int x0, int y0, int x1, int y1, int x2, int y2, int r, int g,int b)
{
    this->r=r;
    this->g=g;
    this->b=b;
    this->x0=x0;
    this->y0=y0;
    this->x1=x1;
    this->y1=y1;
    this->x2=x2;
    this->y2=y2;
    this->redraw();
}

clear_widget_c::clear_widget_c(int w, int h):
Fl_Widget(0,0,w,h)
{
    
}

void clear_widget_c::draw()
{
    fl_color(FL_WHITE);
    fl_rectf(x(),y(),w(),h());
}

void clear_widget_c::flush()
{
    this->redraw();
}

text_c::text_c(int w, int h):
Fl_Widget(0,0,w,h),str("")
{

}

void text_c::draw()
{
    // printf("test1:%s,r:%d,g:%d,b:%d,font:%d,x0:%d,y0:%d\n",this->str,r,g,b,font,x0,y0);
    fl_color(this->r,this->g,this->b);
    fl_font(FL_HELVETICA,font);
    fl_draw(this->str,this->x0,this->y0);
}

void text_c::add(int x0, int y0, int r, int g, int b, int font,const char* str)
{
    // printf("test2:%s,r:%d,g:%d,b:%d,font:%d,x0:%d,y0:%d\n",this->str,r,g,b,font,x0,y0);
    this->x0=x0;
    this->y0=y0;
    this->r=r;
    this->g=g;
    this->b=b;
    this->font=font;
    this->str=str;
    this->redraw();
}
