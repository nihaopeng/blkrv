#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "vmem.h"
#include "utils.h"
#include <deque>
#include <mutex>
class keyboard:public vmem
{
public:
    int is_active;
    std::deque<uint8_t> pending;   // 未读走的按键数据 (IRQ 保持直到被 CPU 读走)
    bool irq_pending;              // IRQ 当前是否处于拉高状态
    keyboard(uint32_t size);
    ~keyboard();
    // 每 tick 轮询输入并触发中断, 读写走 vmem 基类
    int process(Bus* bus,uint32_t tick=0);
    uint32_t read(uint32_t offset, uint8_t op_type) override;
    // 窗口线程 (ray2d poll_keyboard) 把按键塞入 FIFO 并拉高 IRQ
    void push_key(uint8_t b);
private:
    std::mutex mtx;                // 窗口线程与仿真主线程共享 FIFO 的互斥锁
};


#endif // !_KEYBOARD_H_
