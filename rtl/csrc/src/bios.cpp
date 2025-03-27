#include "bios.h"

bios::bios(std::string mem_file_path,uint32_t size):nvmem(mem_file_path,size){

}

bios::~bios(){
    
}

int bios::process(rib* rib,uint32_t tick){
    if(rib->s0_req){
        switch(rib->s0_mem_op_type){
            case 0:rib->s0_read_data=uint8_t(this->getB(rib->s0_addr));break;
            case 1:rib->s0_read_data=uint16_t(this->get2B(rib->s0_addr));break;
            case 2:rib->s0_read_data=uint32_t(this->get4B(rib->s0_addr));break;
            default:break;
        }
        // printf("%d\n",top->s0_read_data);
        rib->s0_read_valid=1;
    }
    return 0;
}