#ifndef _DEVICES_H_
#define _DEVICES_H_
#include "ram.h"
#include "keyboard.h"
#include "screen.h"
#include "bios.h"
#include "flash.h"
#include "net_card.h"
#include "pmc.h"
#include "graphics.h"
// #include "monitor.h"

class devices
{
    
public:
    bios* my_bios;
    keyboard* my_keyboard;
    ram* my_ram;
    flash* my_flash;
    screen* my_screen;
    net_card* my_net_card;
    // monitor* my_monitor;
    #ifdef ENABLE_GPU
        gpu* my_gpu;
    #endif // ENABLE_GPU
    pmc* my_pmc;
    devices();
    ~devices();
    int process(rib* rib,uint32_t tick=0);
};

#endif // !_DEVICES_H_
