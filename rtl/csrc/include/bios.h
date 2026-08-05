#ifndef _BIOS_H_
#define _BIOS_H_

#include "nvmem.h"

class bios:public nvmem
{
public:
    bios(std::string mem_file_path,uint32_t size);
    ~bios();
    // bios 只读, 屏蔽写操作
    void write(uint32_t offset, uint32_t data, uint8_t op_type) override;
};

#endif // !_BIOS_H_
