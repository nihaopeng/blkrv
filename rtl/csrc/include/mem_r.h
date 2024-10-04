#ifndef _MEM_R_H_
#define _MEM_R_H_
#include "mem.h"

class mem_r:public mem
{
public:
    mem_r(std::string mem_file_path,uint32_t size);
    ~mem_r();
    uint8_t getB(uint32_t pointer);
    uint16_t get2B(uint32_t pointer);
    uint32_t get4B(uint32_t pointer);
    void process(Vtop* top) override;
};

#endif // !_MEM_R_H_
