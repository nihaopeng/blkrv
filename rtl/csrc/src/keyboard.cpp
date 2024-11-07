#include "keyboard.h"

keyboard::keyboard(uint32_t size):vmem(size){
}
keyboard::~keyboard(){
}
void keyboard::process(Vtop* top){
    top->int_port2=0;
    top->s2_read_valid=0;
    if(utils::kbhit()){
        top->int_port2=1;
        char ch=getchar();
        // std::cout<<ch<<std::endl;
        this->putB(0,ch);
        top->s2_read_valid=1;
    }
    if(top->s2_req){
        if(top->s2_we){
            switch(top->s2_mem_op_type){
                case 0:this->putB(top->s2_addr,uint8_t(top->s2_write_data));break;
                case 1:this->put2B(top->s2_addr,uint16_t(top->s2_write_data));break;
                case 2:this->put4B(top->s2_addr,uint32_t(top->s2_write_data));break;
                default:break;
            }
        }else{
            switch(top->s2_mem_op_type){
                case 0:top->s2_read_data=uint8_t(this->getB(top->s2_addr));break;
                case 1:top->s2_read_data=uint16_t(this->get2B(top->s2_addr));break;
                case 2:printf("key1:%x\n%x\n",top->s2_addr,this->get4B(top->s2_addr));top->s2_read_data=uint32_t(this->get4B(top->s2_addr));break;
                default:break;
            }
            // printf("%d",top->s2_read_data);
        }
    }
}