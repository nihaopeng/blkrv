#include "bios.h"

bios::bios(std::string mem_file_path,uint32_t size):nvmem(mem_file_path,size){

}

bios::~bios(){
    
}

void bios::process(Vtop* top){
    if(top->s0_req){
        switch(top->s0_mem_op_type){
            case 0:top->s0_read_data=uint8_t(this->getB(top->s0_addr));break;
            case 1:top->s0_read_data=uint16_t(this->get2B(top->s0_addr));break;
            case 2:top->s0_read_data=uint32_t(this->get4B(top->s0_addr));break;
            default:break;
        }
        // printf("%d\n",top->s0_read_data);
        top->s0_read_valid=1;
    }
}