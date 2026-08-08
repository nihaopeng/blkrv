#ifndef _SCREEN_H_
#define _SCREEN_H_

#include"vmem.h"

#define SCREEN_MAX_W 512
#define SCREEN_MAX_H 200
#define SCREEN_RENDER_INTERVAL 2000   // 普通输出合并渲染的最小 tick 间隔

class screen:public vmem
{
public:
    screen(uint32_t size);
    ~screen();
    // 每 tick 检查显示缓冲区, 读写走 vmem 基类
    int process(Bus* bus,uint32_t tick=0);
    void flush_all(void);     // 关机前强制输出剩余脏行, 避免合并渲染丢最后一帧
private:
    void query_size(void);    // 查询宿主终端尺寸并写入 W/H 寄存器
    void flush_dirty(uint32_t tick); // 输出脏行 + 光标, 清脏标记
    bool first_render;        // 首次渲染/尺寸变化需要整屏输出
    bool dirty[SCREEN_MAX_H]; // 脏行标记 (普通输出合并)
    uint32_t last_out_tick;   // 上次实际输出 tick, 用于节流
    uint32_t last_row, last_col; // 上次实际输出的光标位置 (检测纯光标移动)
};

#endif // !_SCREEN_H_
