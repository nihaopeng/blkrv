#include "mmu.h"

mmu my_mmu;

extern devices my_devices;

tlb::tlb(){}

tlb::~tlb(){}

uint32_t tlb::insert(uint32_t vir,uint32_t ppn){//TODO:采用什么换入换出策略
    if(this->pages1.size()>=size_pages1){
        this->pages1.erase(this->pages1.begin());
        this->pages1[vir]=ppn;
    }else{
        this->pages1[vir]=ppn;
    }
}

uint32_t tlb::check_tlb1(uint32_t vir){
    std::map<uint32_t,uint32_t>::iterator it=pages1.find(vir);
    if(it != pages1.end()){//命中
        return it->second;//返回页号
    }
    return -1;//未命中
}

uint32_t tlb::check_tlb2(uint32_t vir){
    std::map<uint32_t,uint32_t>::iterator it=pages2.find(vir);
    if(it != pages2.end()){//命中
        return it->second;//返回页号
    }
    return -1;//未命中
}

uint32_t tlb::check(uint32_t vir){
    uint32_t ppn=this->check_tlb1(vir);
    if(ppn!=-1){//tlb1命中
        return ppn;
    }
    ppn=this->check_tlb2(vir);
    if(ppn!=-1){//tlb2命中
        return ppn;
    }
    return ppn;//均未命中
}

mmu::mmu(){}

mmu::~mmu(){}

uint32_t mmu::check_page_list(Vtop* top){//TODO:缺页异常中断添加。
    // printf("miss,satp:%d\n",top->satp);
    uint32_t page_list_base_ppn=(top->satp & 0x000fffff);//top->satp & 0x000fffff提取ppn，一级页表基址
    //ppn<<12获取到一级页表的物理基址。
    // printf("page_list_base_ppn:%d\n",page_list_base_ppn);
    uint32_t page1_index=(page_list_base_ppn<<12)+(((top->load_addr_v&0xffc00000)>>22)<<2);//获取一级页表中的pte
    // printf("page1_index:%d\n",page1_index);
    uint32_t pte1=my_devices.my_ram->get4B(page1_index-0x00100000);//TODO:直接跳过RIB，省时间做法
    // printf("pte1:%d\n",pte1);
    uint32_t page2_index=(pte1&0xfffff000) + (((top->load_addr_v&0x003ff000)>>12)<<2);//前20位为ppn
    // printf("page2_index:%d\n",page2_index);
    uint32_t pte2=my_devices.my_ram->get4B(page2_index-0x00100000);
    // printf("pte2:%d\n",pte2);
    uint32_t ppn=(pte2&0xfffff000)>>12;//前20位为ppn
    this->my_tlb.insert((top->satp & 0xfff00000)|(top->load_addr_v>>12),ppn);
    return ppn;
}

uint32_t mmu::convert(Vtop* top,devices* devices){
    if(top->satp & 0xfff00000){//mmu使能，top->satp & 0xfff00000提取asid标识进程号，0号进程属于内核进程，直接使用物理地址。
        // printf("mmu enable,addr:%d\n",top->load_addr_v);
        uint32_t vir=((top->satp & 0xfff00000)|(top->load_addr_v>>12));//vir: asid(12) | vpn(20)
        uint32_t ppn=this->my_tlb.check(vir);
        if(ppn!=-1){//命中
            // printf("hit:%d,%d\n",top->load_addr_v,(ppn<<12) | (top->load_addr_v & 0x00000fff));
            return (ppn<<12) | (top->load_addr_v & 0x00000fff);//物理页号加偏移得到物理基址
        }else{//未命中
            // printf("miss:%d,",top->load_addr_v);
            ppn=this->check_page_list(top);
            // printf("physic_addr:%d\n",(ppn<<12) | (top->load_addr_v & 0x00000fff));
            // int a=0;
            // scanf("%d",&a);
            // sleep(2);
            return (ppn<<12) | (top->load_addr_v & 0x00000fff);
        }
        return 0;
    }else{//mmu不使能
        return top->load_addr_v;
    }
}

