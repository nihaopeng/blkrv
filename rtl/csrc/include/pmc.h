#ifndef _PMC_H_
#define _PMC_H_

#include "vmem.h"

class pmc:public vmem
{
public:
    pmc(uint32_t size);
    ~pmc();
    int powm(Vtop* top);
};

#endif // !_PMC_H_

