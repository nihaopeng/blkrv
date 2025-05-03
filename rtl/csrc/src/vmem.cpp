#include "vmem.h"

vluint64_t main_time=0;

vmem::vmem(uint32_t size){
    this->size=size;
    this->mem_space=(uint8_t*)malloc(sizeof(uint8_t)*size);
}
vmem::~vmem(){
    free(this->mem_space);
}
uint8_t vmem::getB(uint32_t pointer){
    if(pointer>=this->size){
        printf("\n%u out of mem %u\ncnt:%lu\n",pointer,this->size,main_time);
        // exit(-1);
        return 0;
    }
    return *((uint8_t*)(this->mem_space+pointer));
}
uint16_t vmem::get2B(uint32_t pointer){
    if(pointer>=this->size){
        printf("\n%u out of mem %u\ncnt:%lu\n",pointer,this->size,main_time);
        // exit(-1);
        return 0;
    }
    return *((uint16_t*)(this->mem_space+pointer));
}
uint32_t vmem::get4B(uint32_t pointer){
    if(pointer>=this->size){
        printf("\n%u out of mem %u\ncnt:%lu\n",pointer,this->size,main_time);
        // exit(-1);
        return 0;
    }
    return *((uint32_t*)(this->mem_space+pointer));
}
void vmem::putB(uint32_t pointer,uint8_t data){
    if(pointer>=this->size){
        printf("\n%u out of mem %u\ncnt:%lu\n",pointer,this->size,main_time);
        // exit(-1);
        return;
    }
    *((uint8_t*)(this->mem_space+pointer))=data;
}
void vmem::put2B(uint32_t pointer,uint16_t data){
    if(pointer>=this->size){
        printf("\n%u out of mem %u\ncnt:%lu\n",pointer,this->size,main_time);
        // exit(-1);
        return;
    }
    *((uint16_t*)(this->mem_space+pointer))=data;
}
void vmem::put4B(uint32_t pointer,uint32_t data){
    if(pointer>=this->size){
        printf("\n%u out of mem %u\ncnt:%lu\n",pointer,this->size,main_time);
        // exit(-1);
        return;
    }
    *((uint32_t*)(this->mem_space+pointer))=data;
}
int vmem::process(rib* rib,uint32_t tick){
    return 0;
}