#ifndef _NVMEM_H_
#define _NVMEM_H_
#include "mem.h"
#include <fstream>
#include <string>

class nvmem:public mem
{
public:
    std::string mem_file_path;
    std::fstream fp;
    nvmem(std::string mem_file_path,uint32_t size);
    virtual ~nvmem();
    uint8_t getB(uint32_t pointer);
    uint16_t get2B(uint32_t pointer);
    uint32_t get4B(uint32_t pointer);
    void putB(uint32_t pointer,uint8_t data);
    void put2B(uint32_t pointer,uint16_t data);
    void put4B(uint32_t pointer,uint32_t data);
    void sync();
    uint32_t read(uint32_t offset, uint8_t op_type) override;
    void write(uint32_t offset, uint32_t data, uint8_t op_type) override;
};

#endif // !_MEM_R_H_
