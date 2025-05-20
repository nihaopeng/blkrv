#ifdef ENABLE_GPU

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Image_Surface.H>
#include <FL/Fl_RGB_Image.H>
#include <FL/Fl_JPEG_Image.H>  // 新增JPEG支持头文件
#include <FL/Fl_PNG_Image.H>
#include <cmath>  // 用于 sin 和 cos 函数
#include "rib.h"
#include "keyboard.h"

class MyWindow : public Fl_Window {
public:
    rib* my_rib;
    keyboard* my_keyboard;
    MyWindow(int w, int h, const char* title) : Fl_Window(w, h, title) {}

    int handle(int event) override;
};

class BufferedWidget : public Fl_Box {
public:
    Fl_Image_Surface* img_surf;  // 离屏绘图缓冲区
    Fl_RGB_Image* cached_image; // 缓存的图像数据

    void clear_screen();

    void triangle(int x0,int y0,int x1,int y1,int x2,int y2,int r,int b,int g);

    void text(int x0,int y0,int r,int b,int g,int font,const char* str);

    void draw_jpg(unsigned char* jpg_data, size_t data_length,int x, int y);

    void draw_png(unsigned char* png_data, size_t data_length,int x, int y);
    
    BufferedWidget(int X, int Y, int W, int H, const char* L = nullptr);

    ~BufferedWidget();

    // 重新绘制缓冲区
    void flush();

    void draw() override;
    
    // int handle(int event) override;
};


#endif // ENABLE_GPU