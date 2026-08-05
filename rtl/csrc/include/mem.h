#ifndef _MEM_H_
#define _MEM_H_

#include "bus.h"

class mem : public Device {
public:
    uint32_t size;
    mem();
    virtual ~mem();
};
#endif // !_MEM_H_
