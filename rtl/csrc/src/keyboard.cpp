#include "keyboard.h"

keyboard::keyboard(uint32_t size):vmem(size){
    this->putB(0,0);
}
keyboard::~keyboard(){
}
void keyboard::process(Vtop* top,uint32_t tick){
    top->int_port2=0;
    top->s2_read_valid=0;
    if(utils::kbhit()){
        this->cache.push(getchar());
        // printf("size:%d,cache:%d",this->cache.size(),this->getB(0));
        // top->int_port2=1;
        // char ch=getchar();
        // // std::cout<<"kbhit"<<std::endl;
        // std::cout<<"kb:,"<<ch<<std::endl;
        // this->putB(0,ch);
        // top->s2_read_valid=1;
    }
    // if(this->getB(0)){
    //     printf("size:%d,cache:%d",this->cache.size(),this->getB(0));
    // }
    if(!this->cache.empty()&&this->getB(0)==0&&top->interrupt_enable){
        // printf("inter:%d",top->interrupt_enable);
        top->int_port2=1;
        this->putB(0,this->cache.front());
        this->cache.pop();
        // printf("tick:%d,size:%d,cache:%d\n",tick,this->cache.size(),this->getB(0));
        // top->s2_read_valid=1;
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
                case 2:top->s2_read_data=uint32_t(this->get4B(top->s2_addr));break;
                default:break;
            }
            // printf("%d",top->s2_read_data);
        }
    }
}