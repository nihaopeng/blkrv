#ifndef _MMU_H_
#define _MMU_H_

#include<fstream>
#include "Vtop.h"
#include "devices.h"

class page{

};

class tlb{
    public:
        tlb();
        ~tlb();
        int is_hit(uint32_t virtual_addr,uint32_t* physic_addr);
};

class mmu{
    public:
        tlb my_tlb;
        mmu();
        ~mmu();
        uint32_t convert(Vtop* top,devices* devices);
};

#endif // !_MMU_H_