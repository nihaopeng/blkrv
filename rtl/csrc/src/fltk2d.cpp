#ifdef ENABLE_GPU

#include "fltk2d.h"

int MyWindow::handle(int event) {
    if (event == FL_KEYDOWN) {
        int key = Fl::event_key();
        // printf("key:%c\n",key);
        this->my_bus->set_irq(2,true);
        this->my_keyboard->put4B(0, key);
        return 1; // 事件已处理
    }
    return Fl_Window::handle(event); // 调用基类的处理函数
}

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

void BufferedWidget::draw_jpg(unsigned char* jpg_data, size_t data_length,int x, int y){
    Fl_Surface_Device* current = Fl_Surface_Device::surface();
    img_surf->set_current();

    // 从内存创建JPEG图像对象
    Fl_JPEG_Image jpg_image(nullptr, jpg_data, data_length);
    if (!jpg_image.fail() && jpg_data != nullptr) {
        jpg_image.draw(x, y);
    }

    current->set_current();
}

void BufferedWidget::draw_png(unsigned char* png_data,size_t data_length,int x, int y)
{
    Fl_Surface_Device* current = Fl_Surface_Device::surface();
    img_surf->set_current();

    // 从内存创建PNG图像对象
    Fl_PNG_Image png_image(nullptr, png_data, data_length);
    if (!png_image.fail() && png_data != nullptr) {
    png_image.draw(x, y);
    }

    current->set_current();
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

// int BufferedWidget::handle(int event) {
//     this->my_bus->set_irq(2,false);
//     if (event == FL_KEYDOWN) {
//         // 获取 ASCII 码和按键状态
//         int key = Fl::event_key();
//         printf("key:%d\n",key);
//         this->my_bus->set_irq(2,true);
//         this->my_keyboard->put4B(0, key);
//         return 1;
//     }
//     return Fl_Widget::handle(event);
// }


#endif // ENABLE_GPU
