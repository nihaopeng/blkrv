#include "ram.h"

ram::ram(uint32_t size):vmem(size){
    
}

ram::~ram(){
}

void ram::process(rib* top,uint32_t tick){
    if(top->s1_req){
        if(top->s1_we){
            switch(top->s1_mem_op_type){
                case 0:this->putB(top->s1_addr,uint8_t(top->s1_write_data));break;
                case 1:this->put2B(top->s1_addr,uint16_t(top->s1_write_data));break;
                case 2:this->put4B(top->s1_addr,uint32_t(top->s1_write_data));
                        // printf("%d:%d:%d\n",top->s1_addr,top->s1_write_data,this->get4B(top->s1_addr));
                        break;
                default:break;
            }
        }else{
            switch(top->s1_mem_op_type){
                case 0:top->s1_read_data=uint8_t(this->getB(top->s1_addr));break;
                case 1:top->s1_read_data=uint16_t(this->get2B(top->s1_addr));break;
                case 2:top->s1_read_data=uint32_t(this->get4B(top->s1_addr));break;
                default:break;
            }
        }
    }
}

