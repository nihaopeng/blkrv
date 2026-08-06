#ifndef _NVMEM_H_
#define _NVMEM_H_
#include "mem.h"
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

class nvmem:public mem
{
public:
    std::string mem_file_path;
    std::fstream fp;
    // 4KB 页缓存: 命中=内存速度, 未命中=一次文件读; 写仅标记脏页, sync/析构统一回写
    std::unordered_map<uint32_t, std::vector<uint8_t>> cache;
    std::unordered_set<uint32_t> dirty;
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
private:
    std::vector<uint8_t>& page(uint32_t idx);
};

#endif // !_MEM_R_H_
