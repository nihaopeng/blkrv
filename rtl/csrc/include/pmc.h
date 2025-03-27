#ifndef _PMC_H_
#define _PMC_H_

#include "vmem.h"

class pmc:public vmem
{
public:
    pmc(uint32_t size);
    ~pmc();
    int process(rib* rib,uint32_t tick=0) override;
};

#endif // !_PMC_H_

