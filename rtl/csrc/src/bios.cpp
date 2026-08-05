#include "bios.h"

bios::bios(std::string mem_file_path,uint32_t size):nvmem(mem_file_path,size){

}

bios::~bios(){

}

void bios::write(uint32_t offset, uint32_t data, uint8_t op_type){
    (void)offset; (void)data; (void)op_type;  // 只读, 忽略写
}
