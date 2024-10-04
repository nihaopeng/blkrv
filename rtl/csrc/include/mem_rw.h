#ifndef _MEM_RW_H_
#define _MEM_RW_H_

#include "mem_r.h"

class mem_rw:public mem_r
{
public:
    mem_rw(std::string mem_file_path,uint32_t size);
    ~mem_rw();
    void putB(uint32_t pointer,uint8_t data);
    void put2B(uint32_t pointer,uint16_t data);
    void put4B(uint32_t pointer,uint32_t data);
    
    void process(Vtop* top) override;
};

#endif // !_MEM_RW_H_
