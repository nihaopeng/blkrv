#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "vmem.h"
#include "utils.h"
#include<queue>

class keyboard:public vmem
{
public:
    keyboard(uint32_t size);
    ~keyboard();
    std::queue<char> cache;
    void process(rib* top,uint32_t tick=0) override;
};

#endif // !_KEYBOARD_H_
