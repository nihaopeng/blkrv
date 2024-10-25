#include "flash.h"

flash::flash(std::string mem_file_path,uint32_t size):nvmem(mem_file_path,size){
}

flash::~flash(){

}

void flash::process(Vtop* top){
    if(top->s6_req){
        if(top->s6_we){
            switch(top->s6_mem_op_type){
                case 0:this->putB(top->s6_addr,uint8_t(top->s6_write_data));break;
                case 1:this->put2B(top->s6_addr,uint16_t(top->s6_write_data));break;
                case 2:this->put4B(top->s6_addr,uint32_t(top->s6_write_data));break;
                default:break;
            }
            // if(top->s6_addr==0x200000){
            //     printf("w:%d,t:%d,r:%d\n",top->s6_write_data,top->s6_mem_op_type,this->getB(top->s6_addr));
            // }
        }else{
            switch(top->s6_mem_op_type){
                case 0:top->s6_read_data=uint8_t(this->getB(top->s6_addr));break;
                case 1:top->s6_read_data=uint16_t(this->get2B(top->s6_addr));break;
                case 2:top->s6_read_data=uint32_t(this->get4B(top->s6_addr));break;
                default:break;
            }
        }
    }
}