#ifdef ENABLE_GPU
#ifndef _RAY2D_H_
#define _RAY2D_H_

// raylib 2D 渲染封装: 替代原 FLTK 的 BufferedWidget。
// 离屏绘制目标为 RenderTexture (GPU framebuffer), 提供与 GPU 设备
// 命令协议一一对应的 2D 原语; 窗口呈现与键盘轮询由 gpu 线程主循环负责。
//
// 双缓冲: target 是仿真线程绘制用的 back buffer, front 是真正呈现到窗口的
// buffer。back 在游戏一帧内被逐条命令增量绘制 (先清白底再画各种图元), 若
// 直接呈现会看到"白屏闪烁 + 半成品画面"; 因此只在 OS 发出 flush 命令 (帧
// 完成) 时把 back 整体拷贝到 front, 窗口始终只呈现完整帧。

#include <raylib.h>
#include <cstdint>
#include <cstddef>

class keyboard;
class Bus;

class RayBuffer {
public:
    RenderTexture2D target;   // back buffer: 仿真线程按 GPU 命令增量绘制
    RenderTexture2D front;    // front buffer: 仅在 flush 时更新, 呈现到窗口
    int width;
    int height;

    RayBuffer(int w, int h);
    ~RayBuffer();

    void clear_screen();
    void triangle(int x0,int y0,int x1,int y1,int x2,int y2,int r,int g,int b);
    void text(int x0,int y0,int r,int g,int b,int font,const char* str);
    void draw_jpg(const unsigned char* jpg_data, size_t data_length, int x, int y);
    void draw_png(const unsigned char* png_data, size_t data_length, int x, int y);
    void flush();   // 兼容原 flush 语义: 空操作 (帧提交由 present 完成)
    void present(); // 帧提交: 把 back buffer 完整拷贝到 front buffer
};

// 轮询 raylib 键盘事件, 写入键盘设备数据寄存器并拉高 IRQ (低字节 = ASCII/控制码)
void poll_keyboard(keyboard* kb, Bus* bus);

#endif // !_RAY2D_H_

#endif // ENABLE_GPU
