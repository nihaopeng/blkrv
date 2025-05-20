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
    int process(rib* rib,uint32_t tick=0) override;
};


#endif // !_KEYBOARD_H_
