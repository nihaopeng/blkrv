#include "mem_rw.h"

mem_rw::mem_rw(std::string mem_file_path,uint32_t size):mem_r(mem_file_path,size){//unit of 'size' is BYTE
}

mem_rw::~mem_rw(){
}

void mem_rw::put4B(uint32_t pointer,uint32_t data){
    fp.seekp(pointer);
    fp.write(reinterpret_cast<const char*>(&data), sizeof(data));
    fp.flush();
}

void mem_rw::put2B(uint32_t pointer,uint16_t data){
    fp.seekp(pointer);
    fp.write(reinterpret_cast<const char*>(&data), sizeof(data));
    fp.flush();
}

void mem_rw::putB(uint32_t pointer,uint8_t data){
    fp.seekp(pointer);
    fp.write(reinterpret_cast<const char*>(&data), sizeof(data));
    fp.flush();
}



void mem_rw::process(Vtop* top){
    
}
