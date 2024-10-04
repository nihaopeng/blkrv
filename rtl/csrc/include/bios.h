#ifndef _BIOS_H_
#define _BIOS_H_

#include "mem_r.h"

class bios:public mem_r
{
public:
    bios(std::string mem_file_path,uint32_t size);
    ~bios();
    void process(Vtop* top) override;
};


#endif // !_BIOS_H_
