#ifndef _VMEM_H_
#define _VMEM_H_
#include "mem.h"

class vmem:public mem
{
public:
    uint8_t* mem_space;
    vmem(uint32_t size);
    ~vmem();
    uint8_t getB(uint32_t pointer);
    uint16_t get2B(uint32_t pointer);
    uint32_t get4B(uint32_t pointer);
    void putB(uint32_t pointer,uint8_t data);
    void put2B(uint32_t pointer,uint16_t data);
    void put4B(uint32_t pointer,uint32_t data);
    int process(rib* rib,uint32_t tick=0) override;
};

#endif // !_MEM_R_H_
