#ifndef _SCREEN_H_
#define _SCREEN_H_

#include"vmem.h"

class screen:public vmem
{
public:
    screen(uint32_t size);
    ~screen();
    int process(rib* rib,uint32_t tick=0) override;
};

#endif // !_SCREEN_H_
