#ifndef _RAM_H_
#define _RAM_H_

#include "vmem.h"

class ram:public vmem
{
public:
    ram(uint32_t size);
    ~ram();
    int process(rib* rib,uint32_t tick=0) override;
};

#endif // !_RAM_H_

