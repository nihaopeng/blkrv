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
#include "bus.h"

class devices
{

public:
    Bus* my_bus;
    bios* my_bios;
    keyboard* my_keyboard;
    ram* my_ram;
    flash* my_flash;
    screen* my_screen;
    net_card* my_net_card;
    #ifdef ENABLE_GPU
        gpu* my_gpu;
    #endif // ENABLE_GPU
    pmc* my_pmc;
    devices(Bus* bus);
    ~devices();
    int process(Bus* bus,uint32_t tick=0);
};

#endif // !_DEVICES_H_
