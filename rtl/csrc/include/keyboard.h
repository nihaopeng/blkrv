#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "vmem.h"
#include "utils.h"
#include <deque>
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
};


#endif // !_KEYBOARD_H_
