#include "rib.h"

rib::rib(){

}

rib::~rib(){
    
}

void rib::get_flag(Vtop* top){
    this->inst_type_o=top->inst_type_o;
}

void rib::set_flag(Vtop* top){
    top->read_valid=1;
    SET_INT(0);
    SET_INT(1);
    SET_INT(2);
    SET_INT(3);
    SET_INT(4);
    SET_INT(5);
    SET_INT(6);
    SET_INT(7);
    SET_INT(8);

    SET_READ_DATA(0);
    SET_READ_DATA(1);
    SET_READ_DATA(2);
    SET_READ_DATA(3);
    SET_READ_DATA(4);
    SET_READ_DATA(5);
    SET_READ_DATA(6);
    SET_READ_DATA(7);
    SET_READ_DATA(8);

    // if(this->s0_read_data){
    //     printf("data:%x\n",this->s0_read_data);
    // }
    
}

void rib::fresh_port(Vtop* top){
    ZERO_REQ(0);
    ZERO_REQ(1);
    ZERO_REQ(2);
    ZERO_REQ(3);
    ZERO_REQ(4);
    ZERO_REQ(5);
    ZERO_REQ(6);
    ZERO_REQ(7);
    ZERO_REQ(8);
}

uint32_t rib::dispatch(Vtop* top,uint32_t addr){
    this->get_flag(top);
    this->fresh_port(top);
    if(addr>=this->bios_start_addr && addr<this->ram_start_addr){
        DISPATCH(0,bios);
    }
    else if(addr>=this->ram_start_addr && addr<this->keyboard_start_addr){
        DISPATCH(1,ram);
    }
    else if(addr>=this->keyboard_start_addr && addr<this->screen_start_addr){
        DISPATCH(2,keyboard);
    }
    else if(addr>=this->screen_start_addr && addr<this->gpu_start_addr){
        DISPATCH(3,screen);
    }
    else if(addr>=this->gpu_start_addr && addr<this->nic_start_addr){
        DISPATCH(4,gpu);
    }
    else if(addr>=this->nic_start_addr && addr<this->flash_start_addr){
        DISPATCH(5,nic);
    }
    else if(addr>=this->flash_start_addr && addr<this->pmc_start_addr){
        DISPATCH(6,flash);
    }
    else if(addr>=this->pmc_start_addr && addr<this->monitor_start_addr){
        DISPATCH(7,pmc);
    }
    else if(addr>=this->monitor_start_addr && addr<this->other_start_addr){
        DISPATCH(8,monitor);
    }
    else{

    }
}

