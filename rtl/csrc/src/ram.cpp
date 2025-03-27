#include "ram.h"

ram::ram(uint32_t size):vmem(size){
    
}

ram::~ram(){
}

int ram::process(rib* rib,uint32_t tick){
    if(rib->s1_req){
        if(rib->s1_we){
            switch(rib->s1_mem_op_type){
                case 0:this->putB(rib->s1_addr,uint8_t(rib->s1_write_data));break;
                case 1:this->put2B(rib->s1_addr,uint16_t(rib->s1_write_data));break;
                case 2:this->put4B(rib->s1_addr,uint32_t(rib->s1_write_data));
                        // printf("%d:%d:%d\n",rib->s1_addr,rib->s1_write_data,this->get4B(rib->s1_addr));
                        break;
                default:break;
            }
        }else{
            switch(rib->s1_mem_op_type){
                case 0:rib->s1_read_data=uint8_t(this->getB(rib->s1_addr));break;
                case 1:rib->s1_read_data=uint16_t(this->get2B(rib->s1_addr));break;
                case 2:rib->s1_read_data=uint32_t(this->get4B(rib->s1_addr));break;
                default:break;
            }
        }
    }
    return 0;
}

