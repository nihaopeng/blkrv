#include "devices.h"

devices::devices(){
    // printf("1");
    std::cout<<"$init bios"<<std::endl;
    this->my_bios=new bios("./devices/bios",1<<20);
    std::cout<<"$init keyboard"<<std::endl;
    this->my_keyboard=new keyboard(1<<20);
    std::cout<<"$init ram"<<std::endl;
    this->my_ram=new ram(1<<28);
    std::cout<<"$init flash"<<std::endl;
    this->my_flash=new flash("./devices/flash",1<<30);
    std::cout<<"$init screen"<<std::endl;
    this->my_screen=new screen(1<<28);
    std::cout<<"$init net_card"<<std::endl;
    this->my_net_card=new net_card(1<<28);
    #ifdef ENABLE_GPU
        std::cout<<"$init gpu"<<std::endl;
        this->my_gpu=new gpu(1<<28);
    #endif // ENABLE_GPU
    std::cout<<"$init pmc"<<std::endl;
    this->my_pmc=new pmc(0);
    std::cout<<"$init monitor"<<std::endl;
    this->my_monitor=new monitor(1<<28,"../data.csv");
    system("make loadmem");
    this->my_bios->sync();
    this->my_flash->sync();
}

devices::~devices(){
}

void devices::process(Vtop* top,uint32_t tick){
    this->my_bios->process(top);
    this->my_keyboard->process(top,tick);
    this->my_ram->process(top);
    this->my_flash->process(top);
    this->my_screen->process(top);
    this->my_net_card->process(top);
    this->my_monitor->process(top);
    #ifdef ENABLE_GPU
        this->my_gpu->process(top);
    #endif // ENABLE_GPU
}
