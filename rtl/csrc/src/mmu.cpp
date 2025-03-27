#include "mmu.h"

mmu my_mmu;

tlb::tlb(){
    
}

tlb::~tlb(){

}

int tlb::is_hit(uint32_t virtual_addr,uint32_t* physic_addr){
    
}

mmu::mmu(){

}

mmu::~mmu(){

}

uint32_t mmu::convert(Vtop* top,devices* devices){
    if(top->satp & 0x80000000){//mmu使能
        uint32_t page_list_base_addr=(top->satp & 0x7fffffff);
        
        return 0;
    }else{//mmu不使能
        return top->load_addr_v;
    }
}

