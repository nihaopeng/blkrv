#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Image_Surface.H>
#include <FL/Fl_RGB_Image.H>
#include <cmath>  // 用于 sin 和 cos 函数

class BufferedWidget : public Fl_Box {
private:
    Fl_Image_Surface* img_surf;  // 离屏绘图缓冲区
    Fl_RGB_Image* cached_image; // 缓存的图像数据
    float angle;  // 当前旋转角度

    // 旋转变换函数
    void rotate_point(float& x, float& y, float angle_rad) {
        float x_new = x * cos(angle_rad) - y * sin(angle_rad);
        float y_new = x * sin(angle_rad) + y * cos(angle_rad);
        x = x_new;
        y = y_new;
    }

    // 绘制旋转的三角形
    void perform_drawing() {
        // 清空背景
        fl_color(FL_WHITE);
        fl_rectf(0, 0, w(), h());

        // 定义三角形的初始顶点
        float x1 = 50, y1 = 50;
        float x2 = 150, y2 = 50;
        float x3 = 100, y3 = 150;

        // 计算旋转角度（每次更新增加一定的角度）
        float angle_rad = angle * M_PI / 180.0f;  // 转换为弧度

        // 旋转三角形的每个顶点
        rotate_point(x1, y1, angle_rad);
        rotate_point(x2, y2, angle_rad);
        rotate_point(x3, y3, angle_rad);

        // 平移三角形使其居中显示
        float centerX = w() / 2.0f;
        float centerY = h() / 2.0f;
        x1 += centerX - 100;
        y1 += centerY - 100;
        x2 += centerX - 100;
        y2 += centerY - 100;
        x3 += centerX - 100;
        y3 += centerY - 100;

        // 绘制旋转后的三角形
        fl_color(FL_RED);
        fl_polygon(x1, y1, x2, y2, x3, y3);
    }

public:
    BufferedWidget(int X, int Y, int W, int H, const char* L = nullptr)
        : Fl_Box(X, Y, W, H, L), img_surf(nullptr), cached_image(nullptr), angle(0.0f) {
        // 初始化缓冲区
        img_surf = new Fl_Image_Surface(W, H);
        redraw_buffer(); // 初始绘制

        // 启动定时器，每50ms调用一次更新函数
        Fl::repeat_timeout(0.05, update_rotation, this); 
    }

    ~BufferedWidget() {
        delete img_surf;       // 释放离屏绘图缓冲区
        delete cached_image;   // 释放图像缓存
    }

    // 更新旋转角度和重新绘制缓冲区
    static void update_rotation(void* userdata) {
        BufferedWidget* widget = (BufferedWidget*)userdata;

        widget->angle += 1.0f;  // 每次更新增加1度旋转
        if (widget->angle >= 360.0f) {
            widget->angle = 0.0f;  // 保持角度在0-360之间
        }

        widget->redraw_buffer();  // 更新缓冲区内容
        widget->redraw();         // 刷新显示
        Fl::repeat_timeout(0.05, update_rotation, userdata); // 继续定时调用
    }

    // 重新绘制缓冲区
    void redraw_buffer() {
        img_surf->set_current();  // 切换到缓冲区上下文

        // 调用封装的绘图逻辑
        perform_drawing();

        // 获取绘图结果
        Fl_RGB_Image* new_image = img_surf->image();

        // 替换旧缓存
        delete cached_image;
        cached_image = new_image;

        Fl_Display_Device::display_device()->set_current();  // 恢复绘图到屏幕

        redraw();
    }

    void draw() override {

        // 绘制缓冲区图像
        if (cached_image) {
            cached_image->draw(x(), y(), w(), h());
        }
    }
};

int main() {
    Fl_Window* window = new Fl_Window(400, 300, "Rotating Triangle Example");

    BufferedWidget* buffered_widget = new BufferedWidget(50, 50, 300, 200, "Rotating Triangle");

    window->end();
    window->show();
    return Fl::run();
}
