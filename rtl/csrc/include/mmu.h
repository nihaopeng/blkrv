#ifndef _MMU_H_
#define _MMU_H_

#include <cstdint>
#include "bus.h"

// TLB 项: 直接映射 (索引=哈希, tag 比较), 仿真实硬件 SRAM 阵列
struct tlb_entry {
    uint32_t tag;   // vpn(20) | asid(12)
    uint32_t ppn;   // 物理页号 (高 20 位)
    bool     valid;
};

class tlb{
    static const uint32_t TLB1_SETS = 64;    // L1 TLB (原 size_pages1)
    static const uint32_t TLB2_SETS = 1024;  // L2 TLB (原 size_pages2)
    tlb_entry pages1[TLB1_SETS];
    tlb_entry pages2[TLB2_SETS];

    static inline uint32_t hash1(uint32_t vir){
        return (vir ^ (vir >> 5) ^ (vir >> 15)) & (TLB1_SETS - 1);
    }
    static inline uint32_t hash2(uint32_t vir){
        return (vir ^ (vir >> 7) ^ (vir >> 17)) & (TLB2_SETS - 1);
    }
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
