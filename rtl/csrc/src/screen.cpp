#include "screen.h"

screen::screen(std::string mem_file_path,uint32_t size):ram(mem_file_path,size){

}

screen::~screen(){   
}

void screen::process(Vtop* top){
    if(top->s3_req){
        if(top->s3_we){
            switch(top->s3_mem_op_type){
                case 0:this->putB(top->s3_addr,uint8_t(top->s3_write_data));break;
                case 1:this->put2B(top->s3_addr,uint16_t(top->s3_write_data));break;
                case 2:this->put4B(top->s3_addr,uint32_t(top->s3_write_data));break;
                default:break;
            }
        }else{
            switch(top->s3_mem_op_type){
                case 0:top->s3_read_data=uint8_t(this->getB(top->s3_addr));break;
                case 1:top->s3_read_data=uint16_t(this->get2B(top->s3_addr));break;
                case 2:top->s3_read_data=uint32_t(this->get4B(top->s3_addr));break;
                default:break;
            }
        }
    }
}