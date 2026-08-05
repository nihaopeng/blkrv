#include "vmem.h"
#include <cstdlib>
#include <cstdio>

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
uint32_t vmem::read(uint32_t offset, uint8_t op_type){
    switch(op_type){
        case 0:return uint8_t(this->getB(offset));
        case 1:return uint16_t(this->get2B(offset));
        case 2:return uint32_t(this->get4B(offset));
        default:return 0;
    }
}
void vmem::write(uint32_t offset, uint32_t data, uint8_t op_type){
    switch(op_type){
        case 0:this->putB(offset,uint8_t(data));break;
        case 1:this->put2B(offset,uint16_t(data));break;
        case 2:this->put4B(offset,uint32_t(data));break;
        default:break;
    }
}
