#ifndef _SCREEN_H_
#define _SCREEN_H_

#include"vmem.h"

class screen:public vmem
{
public:
    screen(uint32_t size);
    ~screen();
    void process(Vtop* top) override;
};

#endif // !_SCREEN_H_
