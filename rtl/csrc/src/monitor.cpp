#include "monitor.h"

monitor::monitor(uint32_t size,std::string data_path):vmem(size){
    this->fp.open(data_path,std::ios::out);
}

monitor::~monitor(){
    this->fp.close();
}

void monitor::process(Vtop* top,uint32_t tick){
    if(top->s8_req){
        if(top->s8_we){
            switch(top->s8_mem_op_type){
                case 0:this->putB(top->s8_addr,uint8_t(top->s8_write_data));break;
                case 1:this->put2B(top->s8_addr,uint16_t(top->s8_write_data));break;
                case 2:this->put4B(top->s8_addr,uint32_t(top->s8_write_data));break;
                default:break;
            }
        }else{
            switch(top->s8_mem_op_type){
                case 0:top->s8_read_data=uint8_t(this->getB(top->s8_addr));break;
                case 1:top->s8_read_data=uint16_t(this->get2B(top->s8_addr));break;
                case 2:top->s8_read_data=uint32_t(this->get4B(top->s8_addr));break;
                default:break;
            }
            // printf("%d",top->s2_read_data);
        }
    }
    if(this->get4B(0)==1)
    {
        // printf("monitor opened\n");
        if(top->s1_req){
            this->fp<<"s1_req"<<std::endl;
        }else if(top->s2_req){
            this->fp<<"s2_req"<<std::endl;
        }
    }
}