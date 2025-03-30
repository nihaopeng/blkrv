#ifndef _MMU_H_
#define _MMU_H_

#include<fstream>
#include<map>
#include "Vtop.h"
#include "devices.h"

class tlb{
    uint32_t size_pages1=64;
    uint32_t size_pages2=1024;
    std::map<uint32_t,uint32_t> pages1;//vir:ppn,对于第一个int，前12位为asid，后20位为vpn
    std::map<uint32_t,uint32_t> pages2;
    public:
        tlb();
        ~tlb();
        uint32_t insert(uint32_t vir,uint32_t ppn);
        uint32_t check_tlb1(uint32_t vir);
        uint32_t check_tlb2(uint32_t vir);
        uint32_t check(uint32_t vir);
};

class mmu{
    public:
        tlb my_tlb;
        mmu();
        ~mmu();
        uint32_t check_page_list(Vtop* top);
        uint32_t convert(Vtop* top,devices* devices);
};

#endif // !_MMU_H_