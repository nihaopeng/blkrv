#include "devices.h"

devices::devices(){
    // printf("1");
    std::cout<<"$init bios"<<std::endl;
    this->my_bios=new bios("./devices/bios",1<<21-1);
    std::cout<<"$init keyboard"<<std::endl;
    this->my_keyboard=new keyboard("./devices/keyboard",4);
    std::cout<<"$init ram"<<std::endl;
    this->my_ram=new ram("./devices/ram",1<<20-1);
    std::cout<<"$init flash"<<std::endl;
    this->my_flash=new flash("./devices/flash",1<<20-1);
    std::cout<<"$init screen"<<std::endl;
    this->my_screen=new screen("./devices/screen",1<<20-1);
    std::cout<<"$init net_card"<<std::endl;
    this->my_net_card=new net_card("./devices/net_card",1<<20-1);
    system("make loadmem");
    this->my_bios->sync();
    this->my_flash->sync();
    this->my_keyboard->sync();
    this->my_net_card->sync();
    this->my_ram->sync();
    this->my_screen->sync();
}

devices::~devices(){
}

void devices::process(Vtop* top){
    this->my_bios->process(top);
    this->my_keyboard->process(top);
    this->my_ram->process(top);
    this->my_flash->process(top);
    this->my_screen->process(top);
    this->my_net_card->process(top);
}
