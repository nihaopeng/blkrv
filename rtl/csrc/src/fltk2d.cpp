#ifdef ENABLE_GPU

#include "fltk2d.h"

void BufferedWidget::clear_screen()
{
    fl_color(FL_WHITE);
    fl_rectf(0,0,w(),h());
}

void BufferedWidget::triangle(int x0, int y0, int x1, int y1, int x2, int y2, int r, int b, int g)
{
    fl_color(r,g,b);
    fl_begin_polygon();
    fl_vertex(x0,y0);
    fl_vertex(x1,y1);
    fl_vertex(x2,y2);
    fl_end_polygon();
}

void BufferedWidget::text(int x0,int y0,int r,int b,int g,int font,const char* str)
{
    fl_color(r,g,b);
    fl_font(FL_HELVETICA,font);
    fl_draw(str,x0,y0);
}

BufferedWidget::BufferedWidget(int X, int Y, int W, int H, const char *L)
: Fl_Box(X, Y, W, H, L), img_surf(nullptr), cached_image(nullptr)
{
    img_surf = new Fl_Image_Surface(W, H);
}

BufferedWidget::~BufferedWidget()
{
    delete img_surf;     // 释放离屏绘图缓冲区
    delete cached_image; // 释放图像缓存
}

void BufferedWidget::flush()
{   
    // 获取绘图结果
    Fl_RGB_Image* new_image = img_surf->image();

    // 替换旧缓存
    delete cached_image;
    cached_image = new_image;

    Fl_Display_Device::display_device()->set_current();  // 恢复绘图到屏幕
    
    redraw();
}

void BufferedWidget::draw()
{
    // 绘制缓冲区图像
    if (cached_image) {
        cached_image->draw(x(), y(), w(), h());
    }
}


#endif // ENABLE_GPU

