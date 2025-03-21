#ifndef _RIB_H_
#define _RIB_H_

#include<fstream>
#include "ram.h"

class rib{
    public:
        rib();
        ~rib();
        uint32_t fetch(uint32_t addr);
};

#endif // !_RIB_H_