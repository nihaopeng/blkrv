#include "flash.h"
extern vluint64_t main_time;

flash::flash(std::string mem_file_path,uint32_t size):nvmem(mem_file_path,size){
}

flash::~flash(){

}

int flash::process(rib* rib,uint32_t tick){
    if(rib->s6_req){
        // if(rib->s6_addr==0x303ffc){
        //         printf("w:%d,t:%d,r:%d,cnt:%lu\n",rib->s6_write_data,rib->s6_mem_op_type,this->get4B(rib->s6_addr),main_time);
        // }
        if(rib->s6_we){
            switch(rib->s6_mem_op_type){
                case 0:this->putB(rib->s6_addr,uint8_t(rib->s6_write_data));break;
                case 1:this->put2B(rib->s6_addr,uint16_t(rib->s6_write_data));break;
                case 2:this->put4B(rib->s6_addr,uint32_t(rib->s6_write_data));break;
                default:break;
            }
            
        }else{
            switch(rib->s6_mem_op_type){
                case 0:rib->s6_read_data=uint8_t(this->getB(rib->s6_addr));break;
                case 1:rib->s6_read_data=uint16_t(this->get2B(rib->s6_addr));break;
                case 2:rib->s6_read_data=uint32_t(this->get4B(rib->s6_addr));break;
                default:break;
            }
        }
    }
    return 0;
}