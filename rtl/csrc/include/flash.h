#ifndef _FLASH_H_
#define _FLASH_H_
#include "nvmem.h"

class flash:public nvmem
{
public:
    flash(std::string mem_file_path,uint32_t size);
    ~flash();
    void process(Vtop* top) override;
};

#endif // !_FLASH_H_
