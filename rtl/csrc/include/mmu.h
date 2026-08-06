#ifndef _MMU_H_
#define _MMU_H_

#include<fstream>
#include<map>
#include "bus.h"

class tlb{
    uint32_t size_pages1=64;
    uint32_t size_pages2=1024;
    std::map<uint32_t,uint32_t> pages1;//vir:ppn,对于第一个int，后12位为asid，前20位为vpn
    std::map<uint32_t,uint32_t> pages2;
    public:
        tlb();
        ~tlb();
        // 上下文切换时必须清空: ASID 会复用 (同一 pid 反复 spawn), 旧映射会错位
        void flush();
        // 只清空指定 ASID 的项 (sfence.vma 语义), asid==0 时全清
        void flush_asid(uint32_t asid);
        int insert(uint32_t vir,uint32_t ppn);
        int check_tlb1(uint32_t vir);
        int check_tlb2(uint32_t vir);
        int check(uint32_t vir);
};

class mmu{
    public:
        Bus* my_bus;
        uint32_t addr=0;
        uint32_t is_hit=0;
        uint32_t is_enable=0;
        tlb my_tlb;
        mmu(Bus* bus);
        ~mmu();
        uint32_t get_addr() const;
        uint32_t get_is_hit() const;
        uint32_t get_is_enable() const;
        uint32_t check_page_list(uint32_t addr, uint32_t satp);
        uint32_t convert(uint32_t addr, uint32_t satp);
};

#endif // !_MMU_H_
