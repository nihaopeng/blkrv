#include "mem_r.h"

mem_r::mem_r(std::string mem_file_path,uint32_t size):mem(mem_file_path,size){//unit of 'size' is BYTE
}

mem_r::~mem_r(){
}

uint32_t mem_r::get4B(uint32_t pointer){
    fp.seekg(pointer);
    uint32_t data;
    fp.read(reinterpret_cast<char*>(&data), sizeof(data));
    return data;
}

uint16_t mem_r::get2B(uint32_t pointer){
    fp.seekg(pointer);
    uint16_t data;
    fp.read(reinterpret_cast<char*>(&data), sizeof(data));
    return data;
}

uint8_t mem_r::getB(uint32_t pointer){
    fp.seekg(pointer);
    uint8_t data;
    fp.read(reinterpret_cast<char*>(&data), sizeof(data));
    return data;
}

void mem_r::process(Vtop* top){
    
}