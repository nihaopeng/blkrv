#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/fl_draw.H>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <cmath>
#include <vector>
class triangle_c : public Fl_Widget{
    public:
        int x0,y0,x1,y1,x2,y2;
        triangle_c(int x0,int y0,int x1,int y1,int x2,int y2,int w,int h);
        void draw() override;
        void add(int x0,int y0,int x1,int y1,int x2,int y2);
};

class clear_widget_c : public Fl_Widget{
    public:
        clear_widget_c(int w,int h);
        void draw() override;
        void flush();
};

class my_window : public Fl_Double_Window {
public:
    triangle_c* triangle;
    clear_widget_c* clearw;
    my_window(int W, int H, const char* title);
};

my_window::my_window(int W, int H, const char *title):Fl_Double_Window(W,H,title)
{
    this->triangle=new triangle_c(0,0,0,0,0,0,W,H);   
    this->clearw=new clear_widget_c(W,H);
}

triangle_c::triangle_c(int x0, int y0, int x1, int y1, int x2, int y2, int w, int h):
Fl_Widget(0,0,w,h),x0(x0),y0(y0),x1(x1),y1(y1),x2(x2),y2(y2)
{
    
}

void triangle_c::draw()
{
    fl_color(FL_RED);
    fl_begin_polygon();
    fl_vertex(x0,y0);
    fl_vertex(x1,y1);
    fl_vertex(x2,y2);
    fl_end_polygon();
}

void triangle_c::add(int x0, int y0, int x1, int y1, int x2, int y2)
{
    this->x0=x0;
    this->y0=y0;
    this->x1=x1;
    this->y1=y1;
    this->x2=x2;
    this->y2=y2;
    redraw();
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

void draw(void* arg){
    my_window* win=static_cast<my_window*>(arg);
    //get event
    uint32_t event=2;
    switch (event)
    {
    case 2:
        printf("draw triangle\n");
        win->triangle->add(10,10,20,20,10,20);
        break;
    default:
        break;
    }
    Fl::repeat_timeout(1/120,draw,arg);
}

int main(){
    my_window* win=new my_window(800,600,"my window");
    Fl::add_timeout(1/120,draw,(void*)win);
    win->show();
    Fl::run();
}