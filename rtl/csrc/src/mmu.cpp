#include "mmu.h"

tlb::tlb(){}

tlb::~tlb(){}

int tlb::insert(uint32_t vir,uint32_t ppn){//TODO:采用什么换入换出策略
    if(this->pages1.size()>=size_pages1){
        this->pages1.erase(this->pages1.begin());
        this->pages1[vir]=ppn;
    }else{
        this->pages1[vir]=ppn;
    }
    return 0;
}

int tlb::check_tlb1(uint32_t vir){
    std::map<uint32_t,uint32_t>::iterator it=pages1.find(vir);
    if(it != pages1.end()){//命中
        return it->second;//返回页号对应地址
    }
    return -1;//未命中
}

int tlb::check_tlb2(uint32_t vir){
    std::map<uint32_t,uint32_t>::iterator it=pages2.find(vir);
    if(it != pages2.end()){//命中
        return it->second;//返回页号对应地址
    }
    return -1;//未命中
}

int tlb::check(uint32_t vir){
    uint32_t ppn=this->check_tlb1(vir);
    if(ppn!=-1){//tlb1命中
        return ppn;
    }
    ppn=this->check_tlb2(vir);
    if(ppn!=-1){//tlb2命中
        return ppn;
    }
    return -1;//均未命中
}

mmu::mmu(Bus* bus){
    this->my_bus=bus;
}

mmu::~mmu(){}

uint32_t mmu::get_addr() const
{
    return this->addr;
}

uint32_t mmu::get_is_hit() const
{
    return this->is_hit;
}

uint32_t mmu::get_is_enable() const
{
    return this->is_enable;
}

uint32_t mmu::check_page_list(uint32_t addr, uint32_t satp){//TODO:缺页异常中断添加。
    uint32_t page_list_base_ppn=(satp & 0xfffff000);//satp & 0x000fffff提取ppn，一级页表基址
    uint32_t page1_index=page_list_base_ppn+(((addr&0xffc00000)>>22)<<2);//获取一级页表中的pte
    uint32_t pte1=this->my_bus->read(page1_index,2);//页表遍历走总线(DMA读), 不再绕过RIB
    uint32_t page2_index=(pte1&0xfffff000) + (((addr&0x003ff000)>>12)<<2);//前20位为ppn
    uint32_t pte2=this->my_bus->read(page2_index,2);
    uint32_t ppn=pte2&0xfffff000;//前20位为ppn
    this->my_tlb.insert((addr&0xfffff000)|(satp&0x00000fff),ppn);
    return ppn;
}

uint32_t mmu::convert(uint32_t addr, uint32_t satp){
    if(satp & 0x00000fff){//mmu使能
        this->is_enable=1;
        uint32_t vir=((addr&0xfffff000)|(satp&0x00000fff));//vir: vpn(20) | asid(12)
        uint32_t ppn=this->my_tlb.check(vir);
        if(ppn!=-1){//命中
            this->is_hit=1;
            return (ppn | (addr & 0x00000fff));//物理页号加偏移得到物理基址
        }else{//未命中
            ppn=this->check_page_list(addr,satp);
            this->is_hit=0;
            return (ppn | (addr & 0x00000fff));
        }
    }else{//mmu不使能
        this->is_enable=0;
        return addr;
    }
}
