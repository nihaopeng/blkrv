#include "devices.h"
#include <iostream>

devices::devices(Bus* bus){
    this->my_bus=bus;
    std::cout<<"$init bios"<<std::endl;
    this->my_bios=new bios("./devices/bios",1<<20);
    std::cout<<"$init keyboard"<<std::endl;
    this->my_keyboard=new keyboard(1<<20);
    std::cout<<"$init ram"<<std::endl;
    this->my_ram=new ram(1<<28);
    std::cout<<"$init flash"<<std::endl;
    this->my_flash=new flash("./devices/flash",1<<29);
    std::cout<<"$init screen"<<std::endl;
    this->my_screen=new screen(1<<28);
    std::cout<<"$init net_card"<<std::endl;
    this->my_net_card=new net_card(1<<28);
    #ifdef ENABLE_GPU
        std::cout<<"$init gpu"<<std::endl;
        this->my_gpu=new gpu(1<<28,this->my_keyboard);
    #endif // ENABLE_GPU
    std::cout<<"$init timer"<<std::endl;
    this->my_timer=new Timer(1<<20, 100000);  // ~100ms interval
    std::cout<<"$init pmc"<<std::endl;
    this->my_pmc=new pmc(0);
    this->my_bios->sync();
    this->my_flash->sync();

    // 注册设备到总线, 顺序即地址解码优先级 (first-match)
    bus->register_dev(my_bios,     0x00000000, 1<<20,   0xFF);
    bus->register_dev(my_ram,      0x00100000, 256<<20, 0xFF);
    bus->register_dev(my_timer,    0x60400000, 1<<20,   7);   // CLINT-style timer, IRQ 7, after all other devices
    bus->register_dev(my_keyboard, 0x10100000, 1<<20,   2);
    bus->register_dev(my_screen,   0x10200000, 256<<20, 0xFF);
    #ifdef ENABLE_GPU
        bus->register_dev(my_gpu,  0x20200000, 256<<20, 0xFF);
    #endif // ENABLE_GPU
    bus->register_dev(my_net_card, 0x30200000, 256<<20, 0xFF);
    bus->register_dev(my_flash,    0x40200000, 512<<20, 0xFF);
    bus->register_dev(my_pmc,      0x60200000, 1<<20,   0xFF);
}

devices::~devices(){
}

int devices::process(Bus* bus,uint32_t tick){
    this->my_keyboard->process(bus,tick);
    this->my_screen->process(bus,tick);
    this->my_net_card->process(bus,tick);
    this->my_timer->process(bus,tick);
    #ifdef ENABLE_GPU
        this->my_gpu->process(bus,tick);
    #endif // ENABLE_GPU

    if(this->my_pmc->should_shutdown){
        return -1;
    }
    return 0;
}
