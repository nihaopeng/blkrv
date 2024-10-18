#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "vmem.h"
#include "utils.h"

class keyboard:public vmem
{
public:
    keyboard(uint32_t size);
    ~keyboard();
    void process(Vtop* top) override;
};

#endif // !_KEYBOARD_H_
