
#include "mem.h"

mem::mem(std::string mem_file_path,uint64_t size){//unit of 'size' is BYTE
    this->inf.open(mem_file_path,std::ios::binary|std::ios::in);
    this->ouf.open(mem_file_path,std::ios::binary|std::ios::out);
    if (!this->ouf||!this->inf) {
        std::cerr << "can not open the file" << std::endl;
        throw std::runtime_error("file open error");
    }
    if(size%4!=0){
        std::cout<<"The memory size you specified is not a multiple of 4, please modify it"<<std::endl;
        throw std::runtime_error("mem alloc error");
    }
    uint32_t d=0;
    //from the start to init the mem;
    for(uint64_t i=0;i<size;i+=4){
        this->ouf.seekp(i);
        this->ouf.write(reinterpret_cast<const char*>(&d),sizeof(d));
    }
}

mem::~mem(){
    this->inf.close();
    this->ouf.close();
}

uint32_t mem::get4B(uint64_t pointer){
    inf.seekg(pointer);
    uint32_t data;
    inf.read(reinterpret_cast<char*>(&data), sizeof(data));
    return data;
}

uint16_t mem::get2B(uint64_t pointer){
    inf.seekg(pointer);
    uint16_t data;
    inf.read(reinterpret_cast<char*>(&data), sizeof(data));
    return data;
}

uint8_t mem::getB(uint64_t pointer){
    inf.seekg(pointer);
    uint8_t data;
    inf.read(reinterpret_cast<char*>(&data), sizeof(data));
    return data;
}

void mem::put4B(uint64_t pointer,uint32_t data){
    ouf.seekp(pointer);
    ouf.write(reinterpret_cast<const char*>(&data), sizeof(data));
    ouf.flush();
}

void mem::put2B(uint64_t pointer,uint16_t data){
    ouf.seekp(pointer);
    ouf.write(reinterpret_cast<const char*>(&data), sizeof(data));
    ouf.flush();
}

void mem::putB(uint64_t pointer,uint8_t data){
    ouf.seekp(pointer);
    ouf.write(reinterpret_cast<const char*>(&data), sizeof(data));
    ouf.flush();
}