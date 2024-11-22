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
        int x0,y0,x1,y1,x2,y2,r,g,b;
        triangle_c(int w,int h);
        void draw() override;
        void add(int x0,int y0,int x1,int y1,int x2,int y2, int r, int g, int b);
};

class text_c : public Fl_Widget{
    public:
        int x0,y0,r,g,b,font;
        const char* str;
        text_c(int w,int h);
        void draw() override;
        void add(int x0,int y0,int r,int g,int b,int font,const char* str);
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
    text_c* text;
    my_window(int W, int H, const char* title);
};

