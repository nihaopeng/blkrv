#ifndef _SCREEN_H_
#define _SCREEN_H_

#include"vmem.h"

class screen:public vmem
{
public:
    screen(uint32_t size);
    ~screen();
    // 每 tick 检查显示缓冲区, 读写走 vmem 基类
    int process(Bus* bus,uint32_t tick=0);
};

#endif // !_SCREEN_H_
