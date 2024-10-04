#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "ram.h"
#include "utils.h"

class keyboard:ram
{
public:
    keyboard(std::string mem_file_path,uint32_t size);
    ~keyboard();
    void process(Vtop* top) override;
};

#endif // !_KEYBOARD_H_
