#include "vmem.h"

vmem::vmem(uint32_t size){
    this->mem_space=(uint8_t*)malloc(sizeof(uint8_t)*size);
}
vmem::~vmem(){
    free(this->mem_space);
}
uint8_t vmem::getB(uint32_t pointer){
    return *((uint8_t*)(this->mem_space+pointer));
}
uint16_t vmem::get2B(uint32_t pointer){
    return *((uint16_t*)(this->mem_space+pointer));
}
uint32_t vmem::get4B(uint32_t pointer){
    return *((uint32_t*)(this->mem_space+pointer));
}
void vmem::putB(uint32_t pointer,uint8_t data){
    *((uint8_t*)(this->mem_space+pointer))=data;
}
void vmem::put2B(uint32_t pointer,uint16_t data){
    *((uint16_t*)(this->mem_space+pointer))=data;
}
void vmem::put4B(uint32_t pointer,uint32_t data){
    *((uint32_t*)(this->mem_space+pointer))=data;
}
void vmem::process(Vtop* top){
    
}