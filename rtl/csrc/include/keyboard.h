#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "vmem.h"
#include "utils.h"
class keyboard:public vmem
{
public:
    int is_active;
    keyboard(uint32_t size);
    ~keyboard();
    // 每 tick 轮询输入并触发中断, 读写走 vmem 基类
    int process(Bus* bus,uint32_t tick=0);
};


#endif // !_KEYBOARD_H_
