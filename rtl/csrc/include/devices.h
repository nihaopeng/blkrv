#ifndef _DEVICES_H_
#define _DEVICES_H_
#include "ram.h"
#include "keyboard.h"
#include "screen.h"
#include "bios.h"
#include "flash.h"
#include "net_card.h"

class devices
{
    
public:
    bios* my_bios;
    keyboard* my_keyboard;
    ram* my_ram;
    flash* my_flash;
    screen* my_screen;
    net_card* my_net_card;
    devices();
    ~devices();
    void process(Vtop* top);
};

#endif // !_DEVICES_H_
