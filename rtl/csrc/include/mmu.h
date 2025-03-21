#ifndef _MMU_H_
#define _MMU_H_

#include<fstream>
#include "ram.h"

class tlb{
    std::vector<>
    public:
        tlb();
        ~tlb();
        int is_hit(uint32_t virtual_addr,uint32_t* physic_addr);
};

class mmu{
    tlb my_tlb;
    public:
        mmu();
        ~mmu();
        uint32_t fetch(uint32_t virtual_addr);
};

#endif // !_MMU_H_