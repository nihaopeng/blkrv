#include "monitor.h"

monitor::monitor(uint32_t size,std::string data_path):vmem(size){
    this->fp.open(data_path,std::ios::out);
    this->fp<<
    "lui,auipc,jal,jalr,bj,load,store,calc,calci,sys,bios,ram,keyboard,screen,nic,flash"
    <<std::endl;
}

monitor::~monitor(){
    this->fp.close();
}

void monitor::process(rib* top,uint32_t tick){
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
        uint32_t inst_type=top->inst_type_o;
        this->fp<<((inst_type&(0x000003ff))>>9)<<","
                <<((inst_type&(0x000001ff))>>8)<<","
                <<((inst_type&(0x000000ff))>>7)<<","
                <<((inst_type&(0x0000007f))>>6)<<","
                <<((inst_type&(0x0000003f))>>5)<<","
                <<((inst_type&(0x0000001f))>>4)<<","
                <<((inst_type&(0x0000000f))>>3)<<","
                <<((inst_type&(0x00000007))>>2)<<","
                <<((inst_type&(0x00000003))>>1)<<","
                <<((inst_type&(0x00000001))>>0)<<","
                <<(uint32_t)top->s0_req<<","
                <<(uint32_t)top->s1_req<<","
                <<(uint32_t)top->s2_req<<","
                <<(uint32_t)top->s3_req<<","
                <<(uint32_t)top->s4_req<<","
                <<(uint32_t)top->s5_req<<","
                <<(uint32_t)top->s6_req<<
                std::endl;
    }
}